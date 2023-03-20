#include "thread_pool.h"

void handler(void *arg)
{
	printf("[%u] is ended.\n",
		(unsigned)pthread_self());

	pthread_mutex_unlock((pthread_mutex_t *)arg); // 释放互斥锁资源
}

void *routine(void *arg)
{
	#ifdef DEBUG
	printf("[%u] is started.\n",
		(unsigned)pthread_self());
	#endif

	thread_pool *pool = (thread_pool *)arg;
	struct task *p;

	while(1)
	{
		/*
		** push a cleanup functon handler(), make sure that
		** the calling thread will release the mutex properly
		** even if it is cancelled during holding the mutex.
		**
		** NOTE:
		** pthread_cleanup_push() is a macro which includes a
		** loop in it, so if the specified field of codes that 
		** paired within pthread_cleanup_push() and pthread_
		** cleanup_pop() use 'break' may NOT break out of the
		** truely loop but break out of these two macros.
		** see line 61 below.
		*/
		//=====================上锁以及预防死锁===========================//
		pthread_cleanup_push(handler, (void *)&pool->lock);  //  把取消处理函数 handler 压入该线程的栈中
		pthread_mutex_lock(&pool->lock);  // 上锁， 获得互斥锁资源
		//================================================//

		// 1, no task, and is NOT shutting down, then wait
		while(pool->waiting_tasks == 0 && !pool->shutdown)  // 判断当前没有任务需要执行 && 当前线程池没有被销毁
		{
			// 进入条件变量中等待任务（睡觉）
			pthread_cond_wait(&pool->cond, &pool->lock);
		}

		// 2, no task, and is shutting down, then exit
		if(pool->waiting_tasks == 0 && pool->shutdown == true)  // 当前没有任务需要执行 && 线程池需要被销毁
		{
			pthread_mutex_unlock(&pool->lock);  // 释放互斥锁资源
			pthread_exit(NULL); // CANNOT use 'break';  退出当前线程
		}

		// 3, have some task, then consume it
		p = pool->task_list->next;  // 让P指向链表中的第一个有效结点
		pool->task_list->next = p->next; // 把第一个有效结点从链表中剔除
		pool->waiting_tasks--;  // 把等待中的任务数量减一

		//================================================//
		pthread_mutex_unlock(&pool->lock);    // 释放互斥锁资源 
		pthread_cleanup_pop(0);// 把压入的取消处理函数从栈中弹出，  并不执行该函数
		//================================================//

		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); // 设置当前线程暂时不接受取消请求
		(p->do_task)(p->arg);  // 执行刚才从链表中取得的任务 （使用函数指针来调用该函数）
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);// 设置当前线程使能取消请求

		free(p);// 释放任务结点
	}

	pthread_exit(NULL);
}

bool init_pool(thread_pool *pool, unsigned int threads_number)
{
	
	pthread_mutex_init(&pool->lock, NULL);// 初始化 互斥锁
	pthread_cond_init(&pool->cond, NULL);// 初始化 条件变量
 
	pool->shutdown = false;  // 把关闭线程池的开关设置为 否 
	pool->task_list = malloc(sizeof(struct task)); // 申请一个任务列表的头结点并让列表指针指向他
	pool->tids = malloc(sizeof(pthread_t) * MAX_ACTIVE_THREADS); // 申请一个堆数据，用于存放线程号

	// 判断前面的堆内存申请是否失败。。。。
	if(pool->task_list == NULL || pool->tids == NULL)
	{
		perror("allocate memory error");
		return false;
	}

	pool->task_list->next = NULL; // 让链表头结点的后继指针指向NULL 

	pool->max_waiting_tasks = MAX_WAITING_TASKS; // 设置线程池任务的最大等待数量
	pool->waiting_tasks = 0;  // 目前任务数量为  0 
	pool->active_threads = threads_number;  // 设置初始状态下线程的数量

	int i;
	for(i=0; i<pool->active_threads; i++)
	{
		if(pthread_create(&((pool->tids)[i]), NULL,
					routine, (void *)pool) != 0)
		{
			perror("create threads error");
			return false;
		}

		#ifdef DEBUG
		printf("[%u]:[%s] ==> tids[%d]: [%u] is created.\n",
			(unsigned)pthread_self(), __FUNCTION__,
			i, (unsigned)pool->tids[i]);
		#endif
	}

	return true;
}

bool add_task(thread_pool *pool,
	      void *(*do_task)(void *arg), void *arg)
{
	struct task *new_task = malloc(sizeof(struct task));  // 申请一个新的结点内存
	if(new_task == NULL)
	{
		perror("allocate memory error");
		return false;
	}

	// 初始化链表的数据域
	new_task->do_task = do_task;  
	new_task->arg = arg;

	// 初始化链表的指针域
	new_task->next = NULL;

	//============ LOCK =============//
	pthread_mutex_lock(&pool->lock);  // 申请获得互斥锁，等待获得操作链表的权限
	//===============================//

	if(pool->waiting_tasks >= MAX_WAITING_TASKS) // 判断任务等待列表是佛已满
	{
		pthread_mutex_unlock(&pool->lock);  // 如果任务列表已满 则直接解锁退出即可

		fprintf(stderr, "too many tasks.\n");
		free(new_task); // 释放结点

		return false;
	}
	
	struct task *tmp = pool->task_list;  // 等价于 tmp  = head 

	// 遍历找到链表的末尾
	while(tmp->next != NULL)
		tmp = tmp->next;

	tmp->next = new_task;  // 把新结点插入到链表的末尾去
	pool->waiting_tasks++; // 让等待的任务数量 +1 

	//=========== UNLOCK ============//
	pthread_mutex_unlock(&pool->lock);  // 释放互斥锁
	//===============================//

	#ifdef DEBUG
	printf("[%u][%s] ==> a new task has been added.\n",
		(unsigned)pthread_self(), __FUNCTION__);
	#endif

	// 尝试唤醒一个在条件变量队列中等待的线程
	pthread_cond_signal(&pool->cond);
	return true;
}

int add_thread(thread_pool *pool, unsigned additional_threads)
{
	if(additional_threads == 0)
		return 0;

	//  总数  = 当前数量 + 目标添加数 ；
	unsigned total_threads =
			pool->active_threads + additional_threads;

	int i, actual_increment = 0;
	// 循环创建新的线程  
	for(i = pool->active_threads;
	    i < total_threads && i < MAX_ACTIVE_THREADS;  // i < 目标线程数 && i < 最大线程数
	    i++)
	{
		// 创建线程
		if(pthread_create(&((pool->tids)[i]),
					NULL, routine, (void *)pool) != 0)
		{
			perror("add threads error");

			// no threads has been created, return fail
			if(actual_increment == 0)
				return -1;

			break;
		}
		actual_increment++; 

		#ifdef DEBUG
		printf("[%u]:[%s] ==> tids[%d]: [%u] is created.\n",
			(unsigned)pthread_self(), __FUNCTION__,
			i, (unsigned)pool->tids[i]);
		#endif
	}

	// 刷新当前实际活跃数量
	pool->active_threads += actual_increment;
	return actual_increment; // 返回成功添加的线程数
}

int remove_thread(thread_pool *pool, unsigned int removing_threads)
{
	if(removing_threads == 0) //判断需要删除的数量是否 为 0 
		return pool->active_threads; // 返回当前线程的活跃数量

	//    剩余线程数量     =     当前活跃数量      -   需要删除的数量
	int remaining_threads = pool->active_threads - removing_threads;

	//  确保剩余的数量必须大于或等于 1  
	remaining_threads = remaining_threads > 0 ? remaining_threads : 1;

	int i;
	// 循环取消线程
	for(i=pool->active_threads-1; i>remaining_threads-1; i--)
	{
		// 取消一个线程
		errno = pthread_cancel(pool->tids[i]);

		if(errno != 0)
			break;

		#ifdef DEBUG
		printf("[%u]:[%s] ==> cancelling tids[%d]: [%u]...\n",
			(unsigned)pthread_self(), __FUNCTION__,
			i, (unsigned)pool->tids[i]);
		#endif
	}

	if(i == pool->active_threads-1)
		return -1; // 失败返回 -1 
	else
	{
		pool->active_threads = i+1;
		return i+1;  // 成功则返回当前剩余的活跃数量
	}
}

bool destroy_pool(thread_pool *pool)
{
	// 1, activate all threads
	pool->shutdown = true; // 把线程池管理结构体的销毁标记设置为真
	pthread_cond_broadcast(&pool->cond);  // 广播唤醒所有的线程

	// 2, wait for their exiting
	int i;
	// 循环等待所有线程退出
	for(i=0; i<pool->active_threads; i++)
	{
		// 结合所有的线程
		errno = pthread_join(pool->tids[i], NULL);
		if(errno != 0)
		{
			printf("join tids[%d] error: %s\n",
					i, strerror(errno));
		}
		else
			printf("[%u] is joined\n", (unsigned)pool->tids[i]);
		
	}

	// 3, free memories
	free(pool->task_list);
	free(pool->tids);
	free(pool);

	return true;
}
