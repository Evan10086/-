#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define     SEND_TYPE   0x02
#define     RECV_TYPE   0x01


typedef struct msgType
{
    // 【 必须 】 消息的类型
    long Type ;

    // 消息正文 可以是任何类型的数据
    char Msg [32] ;
}msgType ;


void RecvMsg( int  id)
{
    msgType msg ;
    while(1)
    {
        if(msgrcv(id , &msg , sizeof(msg), RECV_TYPE  , 0 ) > 0 )
        {
            printf("成功收到%ld类型数据：%s\n" , msg.Type , msg.Msg );
        }
    }

}

void SendMsg(int id)
{
    msgType msg = {
        .Type = SEND_TYPE
    };


    while (1)
    {
        bzero (msg.Msg , 32 );
        printf("请输入需要发送的消息内容：\n");
        fgets( msg.Msg , 32 , stdin );

        if(msgsnd( id , &msg , sizeof(msg.Msg) ,  0 ))
        {
            perror("msg send error");
        }
    }
    

}

int main(int argc, char const *argv[])
{
    // 获得一个KEY值 准备用于消息队列
    key_t key1 =  ftok("/home/",     0x01);
    if (key1 == -1 )
    {
        perror("ftok error");
        return -1 ;
    }
    
    printf("key1:%d \n" , key1   );

    // 把key 随对应的通信对象设置为消息队列
    int id = msgget(key1 , IPC_CREAT | 0666 ); // 当指定的通信对象不存在时会自行创建IPC_CREAT
    if (id == -1 )
    {
        perror("get id error");
        return -1 ;
    }

    printf("get id :%d\n" , id );

    // 创建一个新的子进程
    pid_t pid = fork();
    if (pid == 0 )
    {
        RecvMsg(id);
    }
    else if (pid > 0 )
    {
        SendMsg(id);
    }
    
    
    return 0;
}
