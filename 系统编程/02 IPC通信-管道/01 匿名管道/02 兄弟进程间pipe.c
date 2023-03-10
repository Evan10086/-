#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


// 设计该进程为接收进程， 因此接发送端可以关闭避免占用资源
void ChildProcess2( int * fd )
{   

    close( fd[1] ); // 关闭写端


    char * msg = calloc(1 , 128);

    while (1)
    {
        bzero(msg , 128);
        ssize_t ret_val = read (fd[0] , msg , 128);   // 管道文件默认也是阻塞型，因此如果没有数据可以读取则会进入挂起状态 
        printf("[%d]成功读取：%ld字节的数据：%s\n" , getpid() , ret_val , msg );
    }


    return ;
}


// 设计该进程为发送进程， 因此接收端可以关闭避免占用资源
void ChildProcess1( int * fd )
{   

    close( fd[0] ); // 关闭读端


    char * msg = calloc(1 , 128);
    while (1)
    {
        bzero(msg , 128 );
        printf("请输入需要发送的消息：\n");
        fgets( msg , 128 , stdin ); // 从标准输入中读取一行数据， 标准输入默认是阻塞型

        ssize_t ret_val =  write(fd[1] , msg , strlen(msg));
        printf("[%d]成功发送%ld字节数据...\n" ,getpid() , ret_val );

    }


    return ;
}


int main(int argc, char const *argv[])
{

    // 创建匿名管道
    int fd[2] ;
    pipe ( fd )  ;

    // 创建子进程
    pid_t pid = fork( );
    if ( pid > 0 )
    {
        pid = fork( );
        if ( pid == 0 )
        {
            ChildProcess2(fd);
        }

        for (int i = 0; i < 2; i++)
        {
            wait(NULL);
        }
        
        printf("父进程退出...\n");
        
    }
    else if (pid == 0 )
    {
       ChildProcess1(fd);
    }
    else {
        perror("fork error");
    }

    return 0;
}