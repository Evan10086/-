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
    

    // 设置好 接收方的 IP地址信息 （IP+端口+协议）
    struct sockaddr_in toAddr = {
        .sin_family = AF_INET ,     // 选择使用IPV4协议
        .sin_addr = inet_addr("192.168.1.1"), // 设置目标地址为"192.168.1.1" 
        .sin_port = htons(65000)  // 设置端口号为 65000
    };


    // 发送消息
    char * msg = calloc( 1, 128 ) ;

    while (1)
    {
        printf("请输入需要发送的消息..\n");
        fgets(msg , 128 , stdin );

        int ret_val = sendto(sockfd , msg , strlen(msg) , 0 ,
                        (const struct sockaddr *) &toAddr , sizeof(toAddr));
        printf("成功发送%d字节的数据....\n" , ret_val );
    }
    

    // 关闭套接字
    close(sockfd );

    return 0;
}
