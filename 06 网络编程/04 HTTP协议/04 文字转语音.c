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
#include <stddef.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MSG_LEN     1024 

typedef struct NewB
{
    char host [64];
    char url [256];
    char ip [16] ;
}NewB;


int tcpInit( const char * ip  )
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
        .sin_addr.s_addr = inet_addr(ip)  // 服务器的IP地址
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

int SendRequest( int tcp_socket  , const char * Request  )
{

    // 把请求包发送给服务器
    if(send( tcp_socket , Request , strlen(Request) , 0  ) <= 0)
    {
        perror("send error");
        return -1 ;
    }

    printf("请求发送成功，等待回复....\n"  );

    return 0 ;
}

int GteCode( const char * msg )
{
    int code = atoi( msg + 9 );
    printf("code:%d\n" , code );
    return code ;
}

NewB * GetTagIP( const char *msg )
{
    NewB * info = calloc(1, sizeof(NewB)) ;

    memset(info , 0 , sizeof(NewB));

    struct hostent *info_p ;
    char * p1 = strstr( msg ,  "Location" );
    // Location: https://ae01.alicdn.com/kf/HTB1Eq4ObBaE3KVjSZLe760sSFXaj.png

    if (p1 != NULL )
    {

        char * p2 = strchr( p1 , '/' );

        if (p2 != NULL )
        {

            p2 += 2 ;
            // 提取主机名
            for ( int i = 0 ; p2[i] != '/' ; i++)
            {
                (info->host)[i] = p2[i] ;
            }

            // 提取URL 
            // Location: https://ae01.alicdn.com/kf/HTB1Eq4ObBaE3KVjSZLe760sSFXaj.png
            char * p3 = strchr(p2 , '/') ;
            for ( int i = 0 ; p3[i] != '\r' && p3[i+1] != '\n' ; i++)
            {
                (info->url)[i] = p3[i] ;
            }

            printf("host:[%s] , url:[%s]\n" , info->host , info->url);

    
            info_p = gethostbyname(info->host);

            // 列出指定域名的IP地址（列表）
            for(int i=0; info_p->h_addr_list[i] != NULL; i++)
                printf("%s\n", inet_ntoa(*(struct in_addr*)((info_p->h_addr_list)[i])));
            
            strncpy( info->ip , inet_ntoa(*(struct in_addr*)((info_p->h_addr_list)[0])) , 16 );
        }
        


    }
    
    return info ;
}

void saveFile( int newFd , char * FileName )
{

    char * msg = calloc(1, 2048) ;
    int ret_val = 0;
    int count = 0 ;

    // 初始化好一个图像文件
    int fileFd = open ( FileName , O_RDWR | O_TRUNC | O_CREAT , 0666 );
    if (-1 == fileFd )
    {
        perror("open file ");
        exit(0);
    }
    


    while(1)
    {
        if((ret_val = recv( newFd , msg ,2048 , 0 )) > 0)
        {
            printf("msg:%s\n" , msg );
            count += ret_val ;

            // 先找到数据的内容的开始 \r\n \r\n 
            char * FileP = strstr( msg , "\r\n\r\n" );
            if ( FileP != NULL )
            {
                printf("找到文件的正文内容...\n");
                write(fileFd , FileP+4 , ret_val-(FileP - msg)-4 );
                continue; 
            }
            
            write(fileFd , msg , ret_val);


        } 
        else{
            close(fileFd);
            perror("recv 文件传输完成... ");
            break ;
        }

    }

    printf("总共收到%d字节的数据...\n " ,  count);
}

// int RecvMsg( int tcp_socket )
// {
 
//     char * msg = calloc(1, MSG_LEN );
//     // 读取服务器返回的消息
//     if(recv( tcp_socket , msg , MSG_LEN , 0  ))
//     {
//         printf("服务器返回的消息：\n%s\n" , msg );


//         if (GteCode( msg ) == 302)
//         {
//             printf("当前状态码为 302 我们需要 进一步跳转....\n");

//             NewB * info = GetTagIP( msg );
//             printf("host:%s \n url:%s \n ip:%s\n" , info->host ,info->url , info->ip);

//             // 建立一个新的跳转链接
//             int newFd = tcpInit( info->ip) ;

//             char Request [1024]  = {0} ;
//             snprintf( Request , 1024 ,"GET %s HTTP/1.1\r\nHost:%s\r\n\r\n" , info->url , info->host );
//             printf("Request:%s\n" , Request);
//             SendRequest( newFd   ,  Request  );

//             saveFile(newFd , "1.jpg"  );
//         }
//         else if ( GteCode( msg ) == 200 )
//         {
//             saveFile( tcp_socket , "1.mp3"  );
//         }
        
//     }

//     free(msg);

//     return 0 ;
// }


int main(int argc, char const *argv[])
{
    

    // 初始化 TCP 套接字
    int tcp_socket = tcpInit ( "122.114.161.6" ) ;


    // api.dzzui.com/api/tiangou?format=json
    // 发送 HTTP请求包 (准备好请求包)
    // https://api.dzzui.com/api/avatar
    // ae01.alicdn.com/kf/HTB1AIpLbv1H3KVjSZFH762KppXaA.png
    //https://api.qingyunke.com/api.php?key=free&appid=0&msg=今天有点热啊
    char * Request  = "GET /api/tts?txt=等了一个早上就为了这个 HTTP/1.1\r\nHost:api.dzzui.com\r\n\r\n" ;

    if(SendRequest(  tcp_socket  ,  Request  ) )
    {
        printf("请求发送失败。。。。\n");
        exit(0);
    }
  
   
    // RecvMsg( tcp_socket );
    saveFile( tcp_socket , "1.mp3" );




    return 0;
}

