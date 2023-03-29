#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <arpa/inet.h>

#define MSG_LEN     128 
#define ARR_SIZE    5

typedef struct clientAddr
{
    struct sockaddr_in Addr ; // 客户端的地址信息
    int connectFd ;         // 客户端的已连接套接字
    bool valid ;            // 该数据是否有效
} addrArr  ;

addrArr arr [ARR_SIZE] ; 


void showList(void)
{
    printf("*****************************\n");
    printf("当前在线情况：\n");
    for (int i = 0; i < ARR_SIZE; i++)
    {
        if (arr[i].valid)
        {
            printf("[%d]:[%s][%d][%d]\n" , i,
                inet_ntoa(arr[i].Addr.sin_addr) , ntohs(arr[i].Addr.sin_port) , arr[i].connectFd );
        }
        
    }
    printf("*****************************\n");
}

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

int slaveAddr( int connectfd , struct sockaddr_in clienAddr )
{

    // 尝试遍历整个数据组并找到一个合适的位置把数据存进去
    int i ;
    for (i = 0; i < ARR_SIZE ; i++)
    {
        if ( arr[i].valid )
        {
            continue;
        }
        
        break ;
    }
    
    if (i == ARR_SIZE)
    {
        printf("当前服务器已经满载...\n");
        char * msg = "当前服务器繁忙... 请稍后再试...\n";
        send(connectfd , msg , strlen(msg) , 0 );
        close(connectfd);
        return -1 ;
    }

    arr[i].connectFd  = connectfd ;
    arr[i].Addr = clienAddr ;
    arr[i].valid = true ;
    printf("成功连接一个新的客户端[%s][%d][%d]\n" , 
            inet_ntoa(arr[i].Addr.sin_addr) , ntohs(arr[i].Addr.sin_port) , arr[i].connectFd );

    return 0 ;
}

int delArr( int tcp_socket )
{

    for (int i = 0; i < ARR_SIZE ; i++)
    {
        if ( tcp_socket == arr[i].connectFd )
        {
            arr[i].valid = false ;
        }
    }

}

void * RecvMsg (void * arg )
{
    // 把当前线程设置为分离属性  (当该线程退出时不会进入僵尸状态直接死掉并它的内存会被系统回收)
    pthread_detach( pthread_self() );


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

        printf("成功收到[%d]消息%ld字节:%s \n" , tcp_socket , ret_val , msg );
        if ( !strcmp(msg , "byebye" ) )
        {
            close(tcp_socket);
            delArr( tcp_socket );
            showList();
            break ;
        }
        
        
    }
    
    free(msg);
    // return NULL ;
    pthread_exit(NULL);
}

void SendMsg (int tcp_socket )
{
 

    char * msg = calloc(1 , MSG_LEN );

    memset(msg , 0 , MSG_LEN );

    printf("请输入需要发送的消息：\n");
    fgets( msg , MSG_LEN , stdin );

    ssize_t ret_val = write(tcp_socket , msg , strlen(msg) );
    if ( ret_val <= 0 )
    {
        perror("write error");
    }

    printf("成功发送消息%ld字节:%s \n" , ret_val , msg );
    if ( !strcmp(msg , "byebye" ) )
    {
        close(tcp_socket);
        delArr(tcp_socket) ;
        showList();
    }

    
    return ;
}

void * Accetp(void * arg)
{

    int tcp_socket = *(int *)arg ;
    struct sockaddr_in clienAddr ;
    int addrlen = sizeof(clienAddr);

    while(1)
    {

        // [阻塞] 等待连接请求 
        int connectfd = accept(tcp_socket , (struct sockaddr *) &clienAddr, &addrlen );
        if (connectfd ==-1 )
        {
            perror("accept");
            return NULL ;
        }

        // 把给客户端的信息存储下来
        slaveAddr(connectfd ,  clienAddr );
        showList();

        // 开启一个新的线程来等待该客户端发来的消息
        pthread_t tid ;
        pthread_create( &tid , NULL , RecvMsg ,  &connectfd );


    }

}


int main(int argc, char const *argv[])
{

    // tcp 初始化 成功后返回一个监听套接字
    int tcp_socket = tcpInit( atoi(argv[1]) );


    // 创建一个用于连接新用户的线程
    pthread_t tid ;
    pthread_create( &tid , NULL , Accetp ,  &tcp_socket );


    // 发送消息给指定客户端
    while (1)
    {
    
        printf("请选择指定的一个下标进行发送消息：\n");
        int index = -1 ;
        scanf("%d" , &index);
        while( getchar() != '\n');

        SendMsg (arr[index].connectFd ) ;
    }
     



    
    
    return 0;
}
