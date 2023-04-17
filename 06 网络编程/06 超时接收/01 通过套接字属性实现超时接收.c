#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{

    // 创建套接字
    int sockfd = socket(AF_INET , SOCK_DGRAM , 0  );
    if ( sockfd == -1 )
    {
        perror("socket error");
        return -1 ;
    }
    printf("套接字创建成功...\n");
    

    // 绑定地址 （把当前进程的端口号与 IP地址进行绑定）
    struct sockaddr_in myAddr = {
        .sin_family = AF_INET ,
        .sin_addr = INADDR_ANY ,
        .sin_port = htons(65001) 
    };
    socklen_t addrlen = sizeof(myAddr) ;
    if(bind(sockfd, (const struct sockaddr *)&myAddr,addrlen))
    {
        perror("bind error");
        return -1 ;
    }

    printf("绑定成功...\n");

    // 设置加入指定的小组
    struct ip_mreq v;  // 定义一个组播地址的结构体
    bzero(&v,sizeof(v)); // 清空组播地址结构体

    inet_pton(AF_INET,"224.0.0.100",&v.imr_multiaddr); // 把组播地址加入到 组播地址的结构体中
    inet_pton(AF_INET,"192.168.1.1",&v.imr_interface);// 把本机的IP地址加入到 组播地址的结构体中

    setsockopt( sockfd , IPPROTO_IP, IP_ADD_MEMBERSHIP, &v, sizeof(v));


    // 设置超时属性
    struct timeval tv;
    tv.tv_sec  = 3;  // 秒
    tv.tv_usec = 0;  // 微秒
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));




    // 等待消息到达
    char * msg = calloc(1 ,128) ;
    struct sockaddr_in formAddr ;

    while (1)
    {
        memset(msg , 0 , 128);
        printf("等待消息到达...\n");
        ssize_t ret_val = recvfrom( sockfd, msg  , 128 ,  0 ,
                            (struct sockaddr *) &formAddr, &addrlen);
        if (ret_val == -1 )
        {
            perror("recv from ");
            break ;
        }
        
        printf("成功收到【%s】【%d】消息尺寸：%ld 收到消息：%s\n"  , 
                    inet_ntoa(formAddr.sin_addr) , ntohs(formAddr.sin_port), ret_val , msg );
    }
    


    // 关闭套接字
    
    
    return 0;
}
