#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void sighandler ( int arg )
{

    int stat ;
    wait( &stat );

    // 判断当前子进程是否正常退出
    if (WIFEXITED(stat))
    {
        printf("当前子进程是正常退出，他的退出值：%d\n" , WEXITSTATUS(stat));
    }
    else if (WIFSIGNALED(stat))
    {
        printf("当前子进程被信号杀死,具体凶手是：%d\n" , WTERMSIG(stat));
    }


}

int ChildProcess(void)
{

    printf("这里是子进程....\n" );

    sleep(5);
    printf("子进程工作完成主备退出....\n" );

    // 在子进程退出前发送信号告知父进程我要退出（通知父进程来收拾）
    kill( getppid() , SIGHUP ); // 给父进程getppid()  发送一个信号 SIGHUP 

    exit(1) ;
}

int ParentProcess(void)
{
    signal(SIGHUP , sighandler ); // 设置当前进程捕获 SIGHUP  ，并设置该信号的响应函数为 sighandler


    while(1)
    {
        pause(); //  挂起当前进程，直到收到某个信号为止
    }

    exit(0);
}


int main(int argc, char const *argv[])
{

    pid_t pid = fork();
    if ( pid > 0 )
    {
        /* 父进程 */
        ParentProcess();
    }
    else if (pid == 0 )
    {
        /* 子进程 */
        ChildProcess();
    }
    
    
    
    
    return 0;
}
