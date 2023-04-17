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
    

    // 设置地址信息 
    struct sockaddr_in addr ={
        .sin_addr = inet_addr("224.0.0.100") ,
        .sin_family = AF_INET ,
        .sin_port = htons(atoi(argv[1]))
    };

    struct ip_mreq v;  // 定义一个组播地址的结构体
    bzero(&v,sizeof(v)); // 清空组播地址结构体

    inet_pton(AF_INET,"224.0.0.100",&v.imr_multiaddr); // 把组播地址加入到 组播地址的结构体中
    inet_pton(AF_INET,"192.168.1.1",&v.imr_interface);// 把本机的IP地址加入到 组播地址的结构体中

    setsockopt( udp_socket , IPPROTO_IP, IP_ADD_MEMBERSHIP, &v, sizeof(v));


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
