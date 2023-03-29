#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdbool.h>
#include <arpa/inet.h>

#define MSG_SIZE     128 

int tcpInit(short  prot )
{
    // 创建套接字
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if ( tcp_socket == -1 )
    {
        perror("socket ");
        return -1 ;
    }
    perror("socket ");
    


    // 设置服务器的IP地址信息
    struct sockaddr_in serverAddr ={
        .sin_family = AF_INET ,
        .sin_port = htons(prot),
        .sin_addr.s_addr = INADDR_ANY
    };
    socklen_t addrlen = sizeof(serverAddr);

    // 把套接字与地址信息进行绑定
    if( bind(tcp_socket , (const struct sockaddr *) &serverAddr, addrlen))
    {
        perror("bind ");
        return -1; 
    }
    perror("bind ");


    // 设置监听
    if(listen(tcp_socket ,  0 ))
    {
        perror("listen ");
        return -1 ;
    }
    perror("listen ");

    // 把监听套接字设置为非阻塞属性
    long state = fcntl(tcp_socket, F_GETFL); // 获取指定套接字的属性信息
    state |= O_NONBLOCK;     // 对属性信息 state 添加O_NONBLOCK 非阻塞属性
    fcntl(tcp_socket, F_SETFL, state);     // 把已经添加了非阻塞属性的变量设置到描述符sockfd

    return tcp_socket ;
}

int main(int argc, char const *argv[])
{

    // TCP 初始化
    int tcp_socket = tcpInit( atoi(argv[1]) );


    struct sockaddr_in clienAddr ;
    int addrlen = sizeof(clienAddr);
    int arrfd [20] = {0} ;
    int Num = 0 ;
    char * msg = calloc(1, MSG_SIZE );


    // 把标准输入设置为非阻塞属性
    long state = fcntl( STDIN_FILENO , F_GETFL); // 获取指定套接字的属性信息
    state |= O_NONBLOCK;     // 对属性信息 state 添加O_NONBLOCK 非阻塞属性
    fcntl(STDIN_FILENO, F_SETFL, state);     // 把已经添加了非阻塞属性的变量设置到描述符sockfd

    while(1)
    {
        // 检查是否有新的用户需要建立链接
        int connectfd = accept(tcp_socket , (struct sockaddr *) &clienAddr, &addrlen );
        if (connectfd > 0 )
        {
            arrfd[Num] =  connectfd ;

            // 把已连接套接字设置为非阻塞属性
            long state = fcntl( arrfd[Num] , F_GETFL); // 获取指定套接字的属性信息
            state |= O_NONBLOCK;     // 对属性信息 state 添加O_NONBLOCK 非阻塞属性
            fcntl(arrfd[Num], F_SETFL, state);     // 把已经添加了非阻塞属性的变量设置到描述符sockfd

            Num ++ ;
        }

        // 检查指定的客户端是否有数据到达
        for (int i = 0; i < Num ; i++)
        {
            memset(msg , 0 , MSG_SIZE );
            if(recv(arrfd[i], msg , MSG_SIZE , 0 ) > 0)
            {
                printf("收到🐒的消息：%s\n"  , msg );
            }
        }
        

        // 检查是否有数据需要发送
        memset(msg , 0 , MSG_SIZE );
        if(fgets(msg , MSG_SIZE , stdin ))
        {
            for (int i = 0; i < Num ; i++)
            {
                send(arrfd[i], msg  , strlen(msg), 0 );
            }
            
        }

        usleep(20000);

    }
    
    
    return 0;
}
