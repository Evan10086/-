////////////////////////////////////////////////////////
//
//  Copyright(C), 2005-2022, GEC Tech. Co., Ltd.
//  www.yueqian.com.cn  ALL RIGHT RESERVED
//
//  文件: TCP-lowat.c
//  描述: 检测、设置TCP的接收缓冲区水位线
//
///////////////////////////////////////////////////////

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>


int connectFd ;


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

    return tcp_socket ;
}

void recvOOB (int sigNum)
{
    char oob;
    recv( connectFd, &oob, 1, MSG_OOB); //接收对端的OOB带外数据
    printf("紧急数据：%c\n" , oob );

}

int main(int argc, char **argv)
{

    signal(SIGURG, recvOOB); // 捕捉信号 SIGURG
   


    // tcp 初始化 
    int socket_fd = tcpInit( atoi(argv[1]) );


    // 等待客户端连接
     connectFd = accept( socket_fd , NULL , NULL );
    if (connectFd == -1 )
    {
        perror("连接失败..");
        exit(0);
    }
    perror("连接");

    fcntl(connectFd, F_SETOWN, getpid());  // 指定信号的接收者（属主）

    // 设置已连接套接字的水位线
    int rcvLowat = 100 ;
    int len = sizeof(rcvLowat);
    if(setsockopt(connectFd, SOL_SOCKET, SO_RCVLOWAT, &rcvLowat, len) != 0)
    {
        perror("setsockopt() 失败");
        exit(0);
    }
    perror("setsockopt() ");

    char buf[128];

    while(1)
    {
        memset(buf ,0 , 128 );

        int ret_val  = recv( connectFd , buf , 128 , 0);
        printf("成功收到%d个字节的数据:%s\n" , ret_val , buf );


    }







	close(connectFd);
	close(socket_fd);
	return 0;
}