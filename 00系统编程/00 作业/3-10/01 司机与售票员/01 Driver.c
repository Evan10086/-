#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// 34) SIGRTMIN     // 售票员 发送的开车的信号  
// 35) SIGRTMIN+1   // 售票员 发送的靠站的信号  
// 36) SIGRTMIN+2   // 售司机 发送的到达终点的信号 

#define     FIFO_RECV   "/tmp/DriveRecv"
#define     FIFO_SEND   "/tmp/DriveSend"

void DriveACar( int sigNum )
{

    printf("[%d]好的我们这就出发...\n" ,sigNum);

}

void StopTheStation( int sigNum )
{

    printf("[%d]好的我们这就停站...\n" ,sigNum);

}

int main(int argc, char const *argv[])
{
    printf("这里是司机端[%d]\n" , getpid() );
    // 设置司机端 捕获  34 35 号信号并作出对应的操作
    signal( 34 , DriveACar );
    signal( 35 , StopTheStation );


    // 创建一个给售票员发送ID的管道
    if(access( FIFO_SEND , F_OK | W_OK | R_OK ))
    {
        if (access( FIFO_SEND , F_OK ))
        {
            if( mkfifo(FIFO_SEND, 0666))
            {
                perror("mkfifo error");
                return -1 ;
            }
        }
        else{
            chmod(FIFO_SEND , 0666 );
        }
    }

    printf ("管道文件权限检查通过...\n");


    // 创建一个接收管道
    if(access( FIFO_RECV , F_OK | W_OK | R_OK ))
    {
        if (access( FIFO_RECV , F_OK ))
        {
            if( mkfifo(FIFO_RECV, 0666))
            {
                perror("mkfifo error");
                return -1 ;
            }
        }
        else{
            chmod(FIFO_RECV , 0666 );
        }
    }

    // 打开管道文件
    int fd_send = open( FIFO_SEND , O_RDWR );// 使用可读写写的方式打开管道文件，可以立即打开，不需要等待读者的出现（因为自己即是写者也是读者）
    if (fd_send == -1 )
    {
        perror("open send fifo error");
        return -1 ;
    }
    printf("司机端的发送管道已经开启成功...\n");


    // 发送本进程的ID到管道中
    pid_t pid = getpid();
    int ret_val  = write( fd_send , &pid , sizeof(pid_t) );
    printf("成功写入%d字节到管道中....\n" , ret_val);


    // 打开管道文件
    int fd_recv = open( FIFO_RECV , O_RDWR );// 使用可读写写的方式打开管道文件，可以立即打开，不需要等待读者的出现（因为自己即是写者也是读者）
    if (fd_recv == -1 )
    {
        perror("open RECV fifo error");
        return -1 ;
    }
    printf("司机端的接收管道已经开启成功...\n");

    // 等待售票员从管道中发来它的ID 
    ret_val = read( fd_recv , &pid , sizeof(pid_t) );
    printf("成功收到售票员的ID：%d\n" , pid );


    // 关闭管道文件  （两个）
    close(fd_recv);
    close(fd_send);

    while(1)
    {
        // 阻塞等待用户输入字符K 
        char ch = getchar() ;
        if (ch == 'H')
        {
            // 发送到达终点的信号
            kill(pid , 36 );
            break;
        }
    }

    


    return 0;
}
