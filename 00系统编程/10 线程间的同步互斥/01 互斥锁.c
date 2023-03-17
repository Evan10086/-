// concurrency.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


int global = 100;

// 定义一个互斥锁资源
pthread_mutex_t  mutex ;


void *isPrime(void *arg)
{
    while(1)
    {
        // 对锁资源进行上锁操作
        printf("%s:正在等待获得锁资源...\n" , __FUNCTION__ );
        pthread_mutex_lock(&mutex);

        printf("%s:成功获得锁资源...\n" , __FUNCTION__ );

        // 访问共享资源的代码区
        if(global%2 == 0)
        {
            printf("%d是偶数\n", global);
        }
        else{
            printf("%d是奇数\n", global);

        }

        // 解锁
        pthread_mutex_unlock(&mutex);
        sleep(1);

    }
}

int main()
{

    // 动态初始化锁资源
    pthread_mutex_init( &mutex , NULL );

    pthread_t tid;
    pthread_create(&tid, NULL, isPrime, NULL);




    // 一条人畜无害的赋值语句
    while(1)
    {
        // 对锁资源进行上锁操作
        printf("%s:正在等待获得锁资源...\n" , __FUNCTION__ );
        pthread_mutex_lock(&mutex);

        printf("%s:成功获得锁资源...\n" , __FUNCTION__ );

        // 访问共享资源
        global = rand() % 5000;

        sleep(3);


        // 解锁
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}