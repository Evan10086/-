#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void ParentProcess( pid_t pid )
{

    int wstatus ;
    // pid_t ret_val =  waitpid( pid , &wstatus , WUNTRACED ); // 阻塞等待 pid 子进程 进入暂停状态
    pid_t ret_val =  waitpid( pid , &wstatus , WCONTINUED ); // 阻塞等待 pid 子进程 从暂停状态回到就绪态
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
        else {
            printf("子进程进入的暂停状态或从暂停状态出来..\n");
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
    printf("我是子进程：%d\n" , getpid() );

    sleep(10) ;

    while(1)
    {
        printf("我是子进程：%d\n" , getpid() );
        sleep(1);
    }


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
        printf("我是父进程我处于%d进程组..\n" , getpgid( getpid() ));
        ParentProcess(pid);
    }
    else if (pid == 0) // 子进程执行的代码块
    {
        printf("我是子进程我处于%d进程组..\n" , getpgid( getpid() ));

        ChildProcess();
    }

    return 0 ;
}
