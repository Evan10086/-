#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
pthread_cond_t  cond ;

int Balance = 0 ;

void * XiTsai (void * arg)
{

    while(1)
    {
        printf("[%ld]:等待获得访问银行的权限...\n" , pthread_self() );
        pthread_mutex_lock(&mutex);
        printf("[%ld]:成功获得访问银行的权限...\n" , pthread_self() );

        while(Balance < 100)
        {
            printf("[%ld]:余额暂时不足,,,,\n",pthread_self());
            /* 解锁后进入条件变量中等待 */
            // 进入睡眠前该函数会帮我们把  mutex 解锁 
            pthread_cond_wait(&cond , &mutex);
            // 当从该函数跳出时 ，会帮我们把  mutex 上锁 
            printf("[%ld]:余额被修改了，我看看能不能取,,,,\n",pthread_self());
        }
        printf("时机成熟，我[%ld]可以去取钱了\n" , pthread_self() );

        sleep(3) ;
        Balance -=100 ;

        printf("我[%ld]成功取走100， 剩下：%d\n" , pthread_self() , Balance );
        
        // 释放互斥锁资源
        pthread_mutex_unlock(&mutex);
    }

}

int main(int argc, char const *argv[])
{
    
    // 初始化 互斥锁  条件变量
    pthread_mutex_init(&mutex, NULL );
    pthread_cond_init(&cond, NULL );

    // 创建线程来取钱
    pthread_t thread;
    for (int i = 0; i < 5 ; i++)
    {
        pthread_create(&thread, NULL , XiTsai , NULL );
    }
    


    // 不定时往 银行中 存钱
    int tmp = 0 ;
    while(1)
    {

        printf("请输入需要存入的金额:\n");
        if(scanf("%d" , &tmp )!= 1)
        {
            printf("请输入正确的金额...\n");
            while(getchar()!= '\n');
            continue; 
        }
        while(getchar()!= '\n');

        printf("你大爷:等待获得访问银行的权限...\n"  );
        pthread_mutex_lock(&mutex);
        printf("你大爷:成功获得访问银行的权限...\n"  );

        Balance += tmp ;

        // 释放互斥锁资源
        pthread_mutex_unlock(&mutex);
        
        // 唤醒所有正在条件变量中阻塞的线程
        // pthread_cond_broadcast(&cond);
        pthread_cond_broadcast(&cond);
    }
    


    return 0;
}

