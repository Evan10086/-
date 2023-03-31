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
        .sin_addr = inet_addr("120.78.234.61"),
        .sin_port = htons(65000) 
    };
    socklen_t addrlen = sizeof(myAddr) ;
    if(bind(sockfd, (const struct sockaddr *)&myAddr,addrlen))
    {
        perror("bind error");
        return -1 ;
    }

    printf("绑定成功...\n");


    // 等待消息到达
    char * msg = calloc(1 ,128) ;
    struct sockaddr_in formAddr ;

    while (1)
    {
        printf("等待消息到达...\n");
        ssize_t ret_val = recvfrom( sockfd, msg  , 128 ,  0 ,
                            (struct sockaddr *) &formAddr, &addrlen);
        printf("成功收到【%s】【%d】消息尺寸：%ld 收到消息：%s\n"  , 
                    inet_ntoa(formAddr.sin_addr) , ntohs(formAddr.sin_port), ret_val , msg );
    }
    


    // 关闭套接字
    
    
    return 0;
}
