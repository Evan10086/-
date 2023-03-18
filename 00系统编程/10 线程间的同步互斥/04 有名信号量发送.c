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
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>


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

int main(int argc, char const *argv[])
{
    // 初始化共享内存
    shmInfo shm = shmInit(  ); 

    // 初始化信号量
    sem_t * fp_write = sem_open("mySemWrite", O_RDWR | O_CREAT, 0666 , 1);
    sem_t * fp_read = sem_open("mySemRead", O_RDWR | O_CREAT, 0666 , 0);
    if ( fp_write == SEM_FAILED || fp_read == SEM_FAILED )
    {
        perror("sem open error");
        exit(0);
    }
    

    while (1)
    {
        // P 获得写入的权限
        printf("等待获得写入权限...\n");
        sem_wait(fp_write);
        printf("成功获得写入权限...\n");


        
        fgets(shm.map , 4096 , stdin );


        // v 释放读取的权限
        printf("释放可读权限...\n");
        sem_post(fp_read);
    }
    
    

    return 0;
}
