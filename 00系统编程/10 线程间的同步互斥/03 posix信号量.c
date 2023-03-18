#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

sem_t sem_read  ;
sem_t sem_write  ;


void * readMem (void * arg )
{
    char * mem = (char *)arg ;

    while (1)
    {
        // 申请一个 可读入的权限 （P操作）
        printf("子函数线程等待获得读取的权限...\n");
        sem_wait(&sem_read );
        printf("子函数线程成功获得读取的权限...\n");
        
        printf("mem:%s\n" , mem );
        sleep(2);
        
        printf("子函数释放一个可写的权限.... \n");
        sem_post(&sem_write);
    }
    
}



int main(int argc, char const *argv[])
{
    
    // 初始化信号量
    sem_init(&sem_read, 0 , 0 );  // 初始化读的信号量为  0 
    sem_init(&sem_write, 0 , 1 ); // 初始化写的信号量为  1 

    // 设置线程之间的共享内存
    char * mem = calloc(1,128);


    // 创建线程
    pthread_t thread;
    pthread_create(&thread, NULL , readMem , (void *)mem);


    while (1)
    {
        // 申请一个 可写入的权限 （P操作）
        printf("主函数线程等待获得写入的权限...\n");
        sem_wait(&sem_write);
        printf("主函数线程成功获得写入的权限...\n");

        
        // 从键盘获取数据并写入到共享内存中
        fgets(mem , 128 , stdin );

        printf("主函数释放一个可读的权限.... \n");
        sem_post(&sem_read);

    }
    
    


    return 0;
}
