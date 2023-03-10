#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


/*
fd[0]  读端
fd[1]  写端
*/


void ParentProcess( int fd [2] )
{

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

void ChildProcess(int * fd)
{
    
    char * msg = calloc(1,128);

    while (1)
    {
        bzero(msg , 128);
        ssize_t ret_val = read (fd[0] , msg , 128);   // 管道文件默认也是阻塞型，因此如果没有数据可以读取则会进入挂起状态 
        printf("[%d]成功读取：%ld字节的数据：%s\n" , getpid() , ret_val , msg );
    }
    

}

int main(int argc, char const *argv[])
{

    // 先创建管道文件
    int fd [2] ;

    if(pipe( fd ))
    {
        perror("pipe error");
        return -1 ;
    }


    // 创建子进程
    pid_t pid = fork( );
    if ( pid > 0 )
    {
        ParentProcess(fd);
    }
    else if (pid == 0 )
    {
       ChildProcess(fd);
    }
    else {
        perror("fork error");
    }
    

    return 0;
}
