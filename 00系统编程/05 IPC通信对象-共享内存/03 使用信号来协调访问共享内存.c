#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define     MAP_SEEK_WRITE  2048
#define     MAP_SEEK_READ   0


int key ;
int shm_id ;
char * map ;
int pid ; 


void * shmInit( void )
{

    // 获得KEY 
    key = ftok( "./" , 0x01 );
    if (key == -1 )
    {
        perror("ftok ");
        exit(0);
    }
    perror("ftok ");


    // 获得SHM  id 
    shm_id = shmget( key , 4096 , IPC_CREAT | 0666 );
    if (-1 == shm_id)
    {
        perror("shm get ");
        exit(0);
    }
    perror("shm get ");
    

    // 映射到本进程
    map = shmat(shm_id , NULL , 0 );
    if (map == (void *)-1)
    {
        perror("shm at ");
        exit(0);
    }
    perror("shm at ");

    bzero(map , 4096);
    
    // 初始化写入自己的ID 到 共享内存中
    *(int * )(map+MAP_SEEK_WRITE) = getpid(); 

    // 返回映射后的入口指针 
    return map ;
}


void writeMem(int sigNum)
{
    printf("收到信号可以进行写入操作，请输入内容：\n");
    
    // 从键盘获取数据并写入到指定 内存中
    fgets(map+MAP_SEEK_WRITE , 2048 , stdin);


    // 发送信号通知对方可以进行读取操作
    kill( pid ,  SIGUSR2 ) ;
}

void readMem(int sigNum)
{

    printf("收到2号信号正在读取消息....\n");

    printf("map:%s\n" , map+MAP_SEEK_READ);

}


int main(int argc, char const *argv[])
{
    // 共享内存初始化
    shmInit();

    // 设置捕获读取和写入的信号
    // signal(SIGUSR1 , writeMem );
    signal(SIGUSR2 , readMem );

    while(pid == 0 )
    {
        pid = *(int *)(map+MAP_SEEK_READ) ;
        usleep(5000);
    }

    printf("成功获得对方的PID：%d\n" ,pid);
    kill( pid ,  SIGUSR2 ) ; // 发送信号通知对方，我已经读取到了你的ID 现在你可以尽情写入数据了



    // 等待信号到达，  进行写入或读取的操作
    while(1)
    {
        printf("收到信号可以进行写入操作，请输入内容：\n");
    
        // 从键盘获取数据并写入到指定 内存中
        fgets(map+MAP_SEEK_WRITE , 2048 , stdin);


        // 发送信号通知对方可以进行读取操作
        kill( pid ,  SIGUSR2 ) ;
    }
    
    return 0;
}
