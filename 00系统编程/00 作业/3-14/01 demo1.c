#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/sem.h>

#define     MAP_SEEK_WRITE  0
#define     MAP_SEEK_READ   2048
#define     SEM_ID          0x01
#define     SHM_ID          0x02



// 声明 共享内存的 结构体资源
typedef struct shmInfo
{
    int key ;
    int shm_id ;
    char * map ;

}shmInfo;



shmInfo shmInit( void )
{
    shmInfo shm ; 

    // 获得KEY 
    shm.key = ftok( "./" , SHM_ID );
    if (shm.key == -1 )
    {
        perror("ftok ");
        exit(0);
    }
    perror("ftok ");


    // 获得SHM  id 
    shm.shm_id = shmget( shm.key , 4096 , IPC_CREAT | 0666 );
    if (-1 == shm.shm_id)
    {
        perror("shm get ");
        exit(0);
    }
    perror("shm get ");
    

    // 映射到本进程
    shm.map = shmat(shm.shm_id , NULL , 0 );
    if (shm.map == (void *)-1)
    {
        perror("shm at ");
        exit(0);
    }
    perror("shm at ");

    bzero(shm.map , 4096);
    

    // 返回映射后的入口指针 
    return shm ;
}

int semInit( unsigned int nsems )
{

    // 获得KEY 
    int sem_key = ftok( "./" , SEM_ID );
    if (sem_key == -1 )
    {
        perror("ftok ");
        exit(0);
    }
    perror("ftok ");

    // 获得 sem id 
    int sem_id = semget(sem_key , nsems, IPC_CREAT | 0666 );
    if ( sem_id == -1  )
    {
        perror("sem get ");
        exit(0);
    }
    perror("sem get ");

    // 初始化信号量组中的各个元素的值
    semctl(sem_id , 0 , SETVAL , 1 ); // 设置信号量组中的0号元素的资源量为 1  【可写资源量】
    semctl(sem_id , 1 , SETVAL , 0 ); // 设置信号来组中的1号元素的资源量为 0  【可读资源量】

    semctl(sem_id , 2 , SETVAL , 1 ); // 设置信号量组中的2号元素的资源量为 1  【可写资源量】
    semctl(sem_id , 3 , SETVAL , 0 ); // 设置信号来组中的3号元素的资源量为 0  【可读资源量】

    return sem_id ;

}

void sendMem( int sem_id , shmInfo shm  )
{

    while(1)
    {
        printf("等待申请得到写入内存的权限...\n");
        // 进入临界区前要申请资源   P操作
        struct sembuf sopsp ={
            .sem_num = 0 ,   // 设置需要操作的信号量数组的元素下标  【可写资源】
            .sem_op = -1  // 设置操作的资源量为 -1  （p 操作 ，申请一个资源） 【申请写入的权限】
        };
        semop(sem_id , &sopsp, 1 );
        printf("等待申请得到写入内存的权限成功，进入共享内存写入数据...\n");


        // 读取共享内存
        fgets(shm.map+MAP_SEEK_WRITE , 4096 , stdin );


        // 离开临界区应该释放资源 V操作
        sopsp.sem_num = 1 ;  // 设置需要操作的信号量数组元素的下标为  1 【可读资源】
        sopsp.sem_op = 1 ; // 设置操作资源量为 1  (v 操作 ， 释放一个资源) 【释放了一个可读的资源】
        semop(sem_id , &sopsp, 1 );
        printf("释放一个可读资源...\n");

    }

}

void readMem( int sem_id , shmInfo shm)
{
    while(1)
    {
        printf("等待申请得到读取内存的权限...\n");
        // 进入临界区前要申请资源   P操作
        struct sembuf sopsp ={
            .sem_num = 3 ,   // 设置需要操作的信号量数组的元素下标  【可写资源】
            .sem_op = -1  // 设置操作的资源量为 -1  （p 操作 ，申请一个资源） 【申请写入的权限】
        };
        semop(sem_id , &sopsp, 1 );
        printf("等待申请得到写入内存的权限成功，进入共享内存读取数据...\n");


        // 读取共享内存
        printf("mem:%s\n" , shm.map+MAP_SEEK_READ );


        // 离开临界区应该释放资源 V操作
        sopsp.sem_num = 2 ;  // 设置需要操作的信号量数组元素的下标为  1 【可读资源】
        sopsp.sem_op = 1 ; // 设置操作资源量为 1  (v 操作 ， 释放一个资源) 【释放了一个可读的资源】
        semop(sem_id , &sopsp, 1 );
        printf("释放一个可读资源...\n");

    }

}

int main(int argc, char const *argv[])
{
    // 共享内存初始化
    shmInfo shm = shmInit(  );

    // 信号量初始化
    int sem_id = semInit( 4 );


    pid_t pid =  fork();
    if (pid == 0)
    {
        readMem(sem_id , shm);
    }
    else if ( pid > 0 )
    {
        sendMem( sem_id , shm );
    }
    
    return 0;
}
