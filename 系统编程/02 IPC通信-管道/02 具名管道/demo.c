#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define     FIFO_PATH_SEND  "/tmp/myFIfo2"
#define     FIFO_PATH_RECV  "/tmp/myFIfo1"

int InitFifo( int fd[2] )
{
    // 创建管道文件
    if(access(FIFO_PATH_SEND , F_OK ))
    {
        if( mkfifo ( FIFO_PATH_SEND , 0666 ))
        {
            perror("管道创建失败....");
            return -1 ;
        }
    }
    
    if(access(FIFO_PATH_RECV , F_OK ))
    {
        if( mkfifo ( FIFO_PATH_RECV , 0666 ))
        {
            perror("管道创建失败....");
            return -1 ;
        }
    }

    // 打开接收端管道
    // 使用可读可写的方式打开管道，可以保证打开管道可以顺利进行，并在运行的过程中不会因为没有读者而导致进程退出
    fd[0] = open( FIFO_PATH_RECV , O_RDWR );  
    if (fd[0] == -1 )
    {
        perror("open fifo error");
        return -1 ;
    }
    else{
        printf("打开接收管道文件成功...\n");
    }

    // 打开发送端管道
    fd[1] = open( FIFO_PATH_SEND , O_RDWR );  
    if (fd[1] == -1 )
    {
        perror("open fifo error");
        return -1 ;
    }
    else{
        printf("打开接收管道文件成功...\n");
    }

    return 0 ;
}

void recvMsg( int fd_recv )
{

    char msg [128] ;

    while (1)
    {
        bzero(msg , 128);
        ssize_t ret_val = read (fd_recv , msg , 128);   // 管道文件默认也是阻塞型，因此如果没有数据可以读取则会进入挂起状态 
        printf("[%d]成功读取：%ld字节的数据：%s\n" , getpid() , ret_val , msg );
    }

}

void sendMsg( int fd_send )
{
    // 把消息写入到写端管道中
    char buf [128];
    while(1)
    {
        bzero(buf, 128);

        printf ("请输入需要发送的数据：\n");
        fgets(buf , 128 , stdin);
        int ret_Val = write( fd_send , buf , strlen(buf)  );
        printf("成功发送 %d 字节的数据....\n" , ret_Val);

    }

}

int main(int argc, char const *argv[])
{

    int fd[2] ;
    // 初始化函数
    if(InitFifo( fd ))
    {
        printf("管道初始化失败....\n");
        return -1 ;
    }

    pid_t pid = fork();
    if ( pid  == 0 )
    {
        recvMsg(fd[0]);
    }
    else if (pid > 0)
    {
        sendMsg( fd[1] );
    }
    

    return 0;
}
