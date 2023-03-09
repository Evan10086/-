#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void ParentProcess( pid_t pid )
{

    int wstatus ;
    pid_t ret_val =  waitpid( pid , &wstatus , 0 ); // 阻塞等待 pid 子进程 退出 
    // pid_t ret_val =  waitpid( pid , &wstatus , WNOHANG  ); //  WNOHANG --》非阻塞等待 pid 子进程 退出 
    if (ret_val > 0 ) // 满足条件 有 指定的某一个子进程处于僵尸状态并成功获得了退出信息
    {
        // 判断当前子进程是否正常退出
        if (WIFEXITED(wstatus))
        {
            printf("当前子进程是正常退出，他的退出值：%d\n" , WEXITSTATUS(wstatus));
        }
        else if (WIFSIGNALED(wstatus))
        {
            printf("当前子进程被信号杀死,具体凶手是：%d\n" , WTERMSIG(wstatus));
        }
    }
    else if (ret_val == 0)
    {
        printf("暂时没有处于 【僵尸状态】 或【状态发生变化】 的子进程\n");
    }
    else 
    {
        perror("wait pid error");
    }
    
    
   return ; 

}

void ChildProcess()
{
    sleep(3);

    printf("我是子进程：%d\n" , getpid() );

    exit(6); // 直接结束本进程
}


int main(int argc, char const *argv[])
{
    pid_t pid = fork();
    if (pid == -1 )
    {
        perror("fork error");
        exit(-1); 
    }

    if (pid > 0 )  // 父进程执行的代码块
    {
        ParentProcess(pid);
    }
    else if (pid == 0) // 子进程执行的代码块
    {
        ChildProcess();
    }

    return 0 ;
}
