#include <stdio.h>
#include <stdbool.h>
 #include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <string.h>
#include <stdlib.h>
 #include <arpa/inet.h>

int main(int argc, char const *argv[])
{


    // 初始化 udp 套接字 
    int udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1)
    {
        perror("socket error");
        exit(0);
    }
    
    int test = 1  ;
    int len ;
    if(getsockopt(udp_socket , SOL_SOCKET , SO_BROADCAST,
                      &test, &len))
    {
        perror("get opt error");
        exit(0);
    }
    printf("test:%d \n" , test);

    // 开启广播功能
    // int flag = 1 ;
    // if(setsockopt(udp_socket , SOL_SOCKET , SO_BROADCAST , &flag, sizeof(flag)))
    // {
    //     perror("开启广播功能失败。。。。\n");
    //     exit(0);
    // }



    // 设置地址信息 （设置广播地址 192.168.1.255 ）
    struct sockaddr_in addr ={
        .sin_addr = inet_addr("192.168.1.255"),
        .sin_family = AF_INET ,
        .sin_port = htons(atoi(argv[1]))
    };


    // 发送广播消息
    char msg [128];
    while(1)
    {

        memset(msg , 0 , 128 );

        fgets(msg , 128 , stdin );

        sendto(udp_socket , msg , strlen(msg) , 0 , (const struct sockaddr *)&addr , sizeof(addr));

    }

    
    
    return 0;
}
