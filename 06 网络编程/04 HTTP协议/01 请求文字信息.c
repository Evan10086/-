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

#define MSG_LEN     1024 

int tcpInit( )
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
        .sin_port = htons( 80 ),  //  HTTP 协议的端口号都是 80 
        .sin_addr.s_addr = inet_addr("122.114.161.6")  // 服务器的IP地址
    };
    socklen_t addrlen = sizeof(serverAddr);


    // 主动发起连接服务的请求
    if( connect( tcp_socket , (const struct sockaddr *)&serverAddr , addrlen ) )
    {
        perror("connect error");
        exit (0);
    }
    
    perror("connect");


    return tcp_socket ;
}


int main(int argc, char const *argv[])
{
    

    // 初始化 TCP 套接字
    int tcp_socket = tcpInit ( ) ;


    // api.dzzui.com/api/tiangou?format=json
    // 发送 HTTP请求包 (准备好请求包)
    // https://api.dzzui.com/api/avatar
    // ae01.alicdn.com/kf/HTB1AIpLbv1H3KVjSZFH762KppXaA.png
    //https://api.qingyunke.com/api.php?key=free&appid=0&msg=今天有点热啊
    char * Request  = "GET /api/avatar HTTP/1.1\r\nHost:api.dzzui.com\r\n\r\n" ;


    // 把请求包发送给服务器
    if(send( tcp_socket , Request , strlen(Request) , 0  ) <= 0)
    {
        perror("send error");
        exit(0);
    }

    printf("请求发送成功，等待回复....\n"  );
    
    char * msg = calloc(1, MSG_LEN );
    // 读取服务器返回的消息
    if(recv( tcp_socket , msg , MSG_LEN , 0  ))
    {
        printf("服务器返回的消息：\n%s\n" , msg );
    }





    return 0;
}

