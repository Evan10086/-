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
    // 声明并初始化一个用于存储消息的字符数组，长度为 MSG_LEN
    char  msg[MSG_LEN] = {0};
    // 将消息字符数组清零
    memset(msg , 0 , MSG_LEN );
    
    // 从 UDP 套接字 udp_socket 接收消息，并将消息存储到 msg 数组中，最多接收 MSG_LEN 字节。
    // 消息来源地址和端口号保存在 FromAddr 变量中，addrlen 存储了 FromAddr 结构体的大小。
    ssize_t ret_val = recvfrom(udp_socket , msg , MSG_LEN  , 0 ,(struct sockaddr *) &FromAddr, &addrlen);
    

    if (ret_val <= 0)  // 如果接收到的消息长度小于等于 0，则打印错误信息
    {
        perror("recv error");    
    }
    
    // 打印接收到的消息及其来源 IP 和端口号
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

    // 绑定套接字到指定的地址和端口号
    if(bind( udp_socket , (const struct sockaddr *) &serverAddr, addrlen ))
    {
        perror("bind ");
        exit (0);
    }
    perror("bind ");

    // 设置信号的接收者为当前进程的 PID
    fcntl(udp_socket, F_SETOWN, getpid() );

    // 将套接字设置为异步工作模式，以允许在收到数据时或连接状态发生变化时发送信号通知进程
    long state  = fcntl(udp_socket, F_GETFL);
    state |= O_ASYNC;
    fcntl(udp_socket, F_SETFL, state);


    // 返回 套接字
    return udp_socket ;
}

int main(int argc, char const *argv[])
{   
    //获取结构体 sockaddr_in 的大小，用于后续操作。
    addrlen = sizeof(struct sockaddr_in) ;

    // 捕捉信号 SIGIO，并指定处理函数为 RecvMsg。
    signal( SIGIO , RecvMsg );

    // UDP_Init 函数用于初始化 UDP 相关设置，将第一个命令行参数转换成整型作为端口号。 
    UDP_Init( atoi(argv[1]) ) ;

    // 发送消息
    char * msg = calloc(1, MSG_LEN );
    while(1)
    {
        printf("请输入需要发送的消息：\n");

        fgets( msg , MSG_LEN , stdin );

        //调用 sendto 函数发送消息。该函数的参数依次为：
        //UDP 套接字、消息内容、消息长度、标志位、目标地址（FromAddr）、目标地址结构体大小。
        ssize_t ret_val = sendto(udp_socket , msg  , strlen(msg ), 0 ,
                      (const struct sockaddr *)&FromAddr , addrlen);

        printf("成功发送%ld字节数据....\n" , ret_val );
    }
    return 0;:pensive:
}
