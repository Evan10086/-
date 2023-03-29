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
#include <signal.h>
#include <arpa/inet.h>

#define MSG_LEN     128 
struct sockaddr_in FromAddr ;
socklen_t addrlen = -1 ;
int udp_socket = -1 ;

void RecvMsg (int sigNum )
{

    char  msg[MSG_LEN] = {0};

    memset(msg , 0 , MSG_LEN );

    ssize_t ret_val = recvfrom(udp_socket , msg , MSG_LEN  , 0 ,(struct sockaddr *) &FromAddr, &addrlen);
    if (ret_val <= 0)
    {
        perror("recv error");
    }
    

    printf("成功收到[%s][%d]消息%ld字节:%s \n" ,  
                inet_ntoa(FromAddr.sin_addr) , ntohs(FromAddr.sin_port)  , ret_val , msg );
    

    
    return  ;
    
}

int UDP_Init( int port ) 
{

    // 创建套接字
    udp_socket = socket(AF_INET , SOCK_DGRAM , 0 );
    if ( udp_socket == -1 )
    {
        perror("socket");
        exit(0);
    }
    
    perror("socket");


    // 设置服务器的IP地址和端口号信息  
    struct sockaddr_in serverAddr ={
        .sin_family = AF_INET ,
        .sin_port = htons(port),
        .sin_addr.s_addr = INADDR_ANY
    };

    // 绑定地址信息
    if(bind( udp_socket , (const struct sockaddr *) &serverAddr, addrlen ))
    {
        perror("bind ");
        exit (0);
    }
    perror("bind ");

    // 设置信号的属主：指定信号的接受者的 PID
    fcntl(udp_socket, F_SETOWN, getpid() );

    // 3. 将套接字设置为异步工作模式：
    long state  = fcntl(udp_socket, F_GETFL);
    state |= O_ASYNC;
    fcntl(udp_socket, F_SETFL, state);


    // 返回 套接字
    return udp_socket ;
}

int main(int argc, char const *argv[])
{   
    addrlen = sizeof(struct sockaddr_in) ;

    // 捕捉信号 SIGIO
    signal( SIGIO , RecvMsg );

    // 初始化 UDP （服务器端 --> 地址的绑定） 
    UDP_Init( atoi(argv[1]) ) ;


    
    // 发送消息
    char * msg = calloc(1, MSG_LEN );
    while(1)
    {

        printf("请输入需要发送的消息：\n");

        fgets( msg , MSG_LEN , stdin );

        ssize_t ret_val = sendto(udp_socket , msg  , strlen(msg ), 0 ,
                      (const struct sockaddr *)&FromAddr , addrlen);
        printf("成功发送%ld字节数据....\n" , ret_val );

    }


    return 0;
}
