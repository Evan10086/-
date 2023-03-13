#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


typedef struct msgType
{
    // 【 必须 】 消息的类型
    long Type ;

    // 消息正文 可以是任何类型的数据
    char Msg [32] ;
}msgType ;


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
    
    
    // 阻止好发送的消息内容  （消息类型 + 消息正文）
    msgType msg ;

    // 发送消息
    msgrcv(id , &msg , sizeof(msg.Msg), 0x01  , 0 );

    printf("收到消息：%s\n" , msg.Msg );


    

    return 0;
}
