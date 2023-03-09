#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

void showID( void )
{
    printf("我是[%d]我处于[%d]进程组..\n" , getpid()  ,getpgid( getpid() ));
    return ;
}

void getChildProcessExitValue( pid_t pid )
{

    int wstatus ;
 
    pid_t ret_val =  waitpid( -pid , &wstatus , 0 );  // 阻塞等待子进程变成僵尸状态
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


void ParentProcess(pid_t pid)
{

    showID(  );
    sleep(1); // 故意放慢脚步等待子进程自立门户创建自己的进程组
    printf("当前子进程的ID:%d\n" , pid );
    getChildProcessExitValue(  pid ) ;

}

void  ChildProcess( void )
{
    showID(  );

    setpgid( 0 , 0 );// 当前进程自己创建一个新的进程组，并把自己加入其中
    // 因此从此以后父子进程将不在同一个小组

    while(1)
    {
        showID(  );
        sleep(1);
    }

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
    
    
    
    
    return 0;
}
