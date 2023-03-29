#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MSG_LEN 128

void * RecvMsg (void * arg )
{
    int tcp_socket = * (int *)arg ;

    char * msg = calloc(1 , MSG_LEN );
    while (1)
    {
        memset(msg , 0 , MSG_LEN );

        ssize_t ret_val = read(tcp_socket , msg , MSG_LEN );
        if ( ret_val <= 0 )
        {
            // 但对端退出下线后 接收函数/读取函数会收到 0 并返回
            perror("read error");
            
            // 关闭套接字释放资源
            close(tcp_socket);  

            // 可以直接结束当前进程
            exit(0) ;
        }

        printf("成功收到消息%ld字节:%s \n" , ret_val , msg );
        
    }


    return NULL ;
}

void SendMsg (int tcp_socket )
{

    char * msg = calloc(1 , MSG_LEN );
    while (1)
    {
        memset(msg , 0 , MSG_LEN );

        printf("请输入需要发送的消息：\n");
        fgets( msg , MSG_LEN , stdin );

        ssize_t ret_val = write(tcp_socket , msg , strlen(msg) );
        if ( ret_val <= 0 )
        {
            perror("write error");
            break ;
        }

        printf("成功发送消息%ld字节:%s \n" , ret_val , msg );
        
    }
    
    return ;
}



int main(int argc, char const *argv[])
{

    // 创建一个套接字
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if ( tcp_socket == -1 )
    {
        perror("socket");
        exit(0); // 直接结束进程
    }

    perror("socket");
    

    // 设置好目标服务器的地址信息
    struct sockaddr_in ServerAddr ={
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = inet_addr( "192.168.1.1") 
    };
    socklen_t addrlen = sizeof(ServerAddr);


    // 发起连接请求
    if(connect(tcp_socket , (const struct sockaddr *)&ServerAddr, addrlen))
    {
        perror("connect ");
        exit(0);
    }
    perror("connect ");


    // 开启新的线程用于接收消息
    pthread_t tid ;
    pthread_create(&tid , NULL , RecvMsg , (void * )&tcp_socket );


    // 为所欲为
    while (1)
    {
        SendMsg( tcp_socket );
    }
    


    return 0;
}
