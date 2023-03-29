#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <pthread.h>

#define MSG_LEN 128

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

int main(int argc, char const *argv[])
{

    // 套接字初始化
    int tcp_socket = tcpInit(atoi(argv[1]) );


    // 定义一个最大描述符
    int max_fd = tcp_socket ;


    // 定义一个读取就绪的集合
    fd_set  set ;

    // 设置多路复用监听
    int Num = 0 ;
    int fdArr [20] = {0};
    struct sockaddr_in clienAddr ;
    char * msg = calloc(1, MSG_LEN );
    socklen_t addrlen = sizeof(clienAddr);

    while(1)
    {

        // 把所有需要监听的描述符都添加到集合中
        FD_ZERO(&set); // 清空集合set 

        // 把标准输入添加到集合 set 中
        FD_SET(STDIN_FILENO , &set);
        
        // 把监听套接字 tcp_socket 添加到集合 set 中
        FD_SET( tcp_socket  , &set);

        // 添加现有所有的已连接套接字
        for (int i = 0; i < Num ; i++)
        {
            FD_SET( fdArr[i]  , &set);

            // 重新刷新当前的最大描述符的值
            max_fd = max_fd > fdArr[i] ? max_fd : fdArr[i] ;
        }
        

        if(select(max_fd + 1, &set, NULL , NULL , NULL ) > 0 )
        {
            // 判断监听套接字是否有数据到达 （有客户端申请连接）
            if ( FD_ISSET(tcp_socket , &set) ) 
            {
                int connectfd = accept(tcp_socket , (struct sockaddr *) &clienAddr, &addrlen );
                if (connectfd ==-1 )
                {
                    perror("accept");
                    return -1 ;
                }

                fdArr[Num] = connectfd ;
                Num ++ ;
            }

            // 判断是否有键盘数据到达 （需要发送数据）
            if (FD_ISSET(STDIN_FILENO , &set) ) 
            {
                memset (msg , 0 , MSG_LEN);
                fgets( msg , MSG_LEN , stdin  );

                for (int i = 0; i < Num; i++)
                {
                    send( fdArr[i] ,msg , strlen(msg) , 0 );
                }
                
            }

            // 判断是否有客户端的数到达
            for (int i = 0; i < Num ; i++)
            {
                if (FD_ISSET( fdArr[i] , &set) ) 
                {
                    memset (msg , 0 , MSG_LEN);
                    if(recv( fdArr[i] ,msg , MSG_LEN , 0 ) == 0 )
                    {
                        // 如果某一个客户端退出
                        /**
                         * 添加把该描述符从数组中剔除的代码
                        */
                    }
                    printf("收到🐛的消息：%s\n"  , msg );
                }
            }

        }
    }



    
    
    return 0;
}
