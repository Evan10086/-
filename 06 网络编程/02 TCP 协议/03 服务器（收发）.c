#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
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
            perror("read error");
            break ;
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
        .sin_port = htons(atoi(argv[1])),
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


    // [阻塞] 等待连接请求 
    struct sockaddr_in clienAddr ;
    int connectfd = accept(tcp_socket , (struct sockaddr *) &clienAddr, &addrlen );
    if (connectfd ==-1 )
    {
        perror("accept");
        return -1 ;
    }
    

    /**
     * connectfd  已连接套接字用于与某一个客户端进行一对一通信
     * 
     * tcp_socket 监听套接字， 用于等待其他客户端进行连接请求
     * 
    */


    perror("accept");

    // 开启新的线程用于接收消息
    pthread_t tid ;
    pthread_create(&tid , NULL , RecvMsg , (void * )&connectfd );


    // 为所欲为
    while (1)
    {
        SendMsg( connectfd );
    }
    


    // 为所欲为
    char * msg = calloc(1,128) ;
    while (1)
    {

        memset( msg , 0 , 128  );

        ssize_t ret_val =  recv(connectfd , msg , 128 , 0 );
        if ( ret_val <= 0 )
        {
            perror("recv ");
            return -1 ;
        }
        printf("成功收到%ld字节的数据:%s\n" , ret_val , msg );
        
    }
    
    // 关闭退出
    close(connectfd );
    close(tcp_socket );
    
    return 0;
}
