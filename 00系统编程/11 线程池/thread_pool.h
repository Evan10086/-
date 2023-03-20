#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <errno.h>
#include <pthread.h>

#define MAX_WAITING_TASKS	1000
#define MAX_ACTIVE_THREADS	20

// 单向不循环链表的结点设计
struct task
{
	void *(*do_task)(void *arg);  // 任务需要执行的函数入口 地址
	void *arg;   					// 任务中所需的参数

	struct task *next;		// 下一个任务结点  （后继结点）
};

typedef struct thread_pool
{
	pthread_mutex_t lock;   // 互斥锁 用于互斥访问任务列表
	pthread_cond_t  cond;   // 条件变量 用于方便阻塞已经唤醒执行任务的线程们

	bool shutdown;		// 是否销毁线程池的开关

	struct task *task_list; // 任务链表 （头指针）

	pthread_t *tids;	// 线程号的数组 （堆中申请）

	unsigned max_waiting_tasks; 	// 最大等待的任务数量
	unsigned waiting_tasks;		// 正在等待被执行的任务数量 
	unsigned active_threads; 	// 当前活跃的线程数 
}thread_pool;


bool init_pool(thread_pool *pool, unsigned int threads_number);
bool add_task(thread_pool *pool, void *(*do_task)(void *arg), void *task);
int  add_thread(thread_pool *pool, unsigned int additional_threads_number);
int  remove_thread(thread_pool *pool, unsigned int removing_threads_number);
bool destroy_pool(thread_pool *pool);

void *routine(void *arg);


#endif
