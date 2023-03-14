#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>

#define     GETID_PATH  "./"
#define     RECV_ID      0x01

typedef struct MsgType
{
    long Type ;

    int Pid ;

}MsgType;


int initQueue( void )
{

    // 获得IPC 通信对象 key值
    key_t key =  ftok( GETID_PATH, RECV_ID );
    if ( -1 == key )
    {
        perror("ftok error");
        // return -1 ;
        exit(0);  // 直接结束进程
    }
    perror("ftok ");

    

    // 把该通信对象设置为 消息队列的功能
    int id = msgget( key, IPC_CREAT | 0666 );
    if (-1 == id )
    {
        perror("msg get  error");
        exit(0);  // 直接结束进程
    }
    perror("msg get ");



    // 返回消息队列的ID值
    return id ;
}

void func(int sigNum , siginfo_t * Info  , void * arg )
{

    sigset_t set ; //定义集合
    sigemptyset(&set);  // 清空集合
    sigfillset(&set); // 把所有的信号添加到集合中

    // 挂起所有信号
    sigprocmask(SIG_SETMASK , &set, NULL );

    // 执行相应动作
    printf("开始处理信号....\n");
    printf("当前收到的信号值：%d, 目标车辆上的乘客数量：%d\n" , sigNum , Info->si_int );
    sleep(10) ;

    // 解除挂起
    sigprocmask(SIG_UNBLOCK , &set, NULL );

}

int main(int argc, char const *argv[])
{
    // 初始化 消息队列 ，用于发送本站台的PID
    int id = initQueue(  );

    // 发送当前进程的PID
    MsgType msg = {
        .Pid = getpid(),
        .Type = RECV_ID 
    };

    if( msgsnd(id , &msg , sizeof( msg.Pid ), 0 ))
    {
        perror("send id error");
        exit(0);
    }
    

    // 等待公交车发来信号  【捕获信号并设置响应函数】
    struct sigaction act = {
        .sa_sigaction = func ,
        .sa_flags = SA_SIGINFO 
    };
    sigaction(SIGUSR1, &act , NULL );

    while (1)
    {
        printf("公交站台它在睡觉.... \n");
        sleep(1);
    }
    

    return 0;
}
