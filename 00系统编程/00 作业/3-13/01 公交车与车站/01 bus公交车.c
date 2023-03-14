#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

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

int main(int argc, char const *argv[])
{

    // 初始化消息队列
    int msg_id = initQueue();

    // 从消息队列中获得公交站台的PID 
    MsgType msg ;
    if( !msgrcv(msg_id , &msg , sizeof(msg), RECV_ID , 0 ))
    {
        perror( "msg recv error");
        exit(0);
    }
    printf("成功获得公交站台的PID:%d\n" , msg.Pid );

    // 销毁消息队列 ？？？？ 
    msgctl(msg_id, IPC_RMID, NULL);

    
    // 等待用户触发信号发送给公交站台
    union sigval value  ;
    while (1)
    {
        printf("请输入当前乘客数量...\n");
        if(scanf("%d" , &value.sival_int ) != 1 )
        {
            while (getchar() != '\n');
            printf("输入有误...\n");
            continue; 
        }
        while (getchar() != '\n');


        sigqueue(msg.Pid , SIGUSR1 , value);

    }

    return 0;
}
