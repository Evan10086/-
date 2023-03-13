#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{

    pid_t pid = fork();

    if (pid > 0 )
    {
        int wstatus ;
        pid_t ret_val = wait(&wstatus);
        if (-1  == ret_val  )
        {
            perror("wait error");
            return -1 ;
        }
        printf( "wstatus:%d\n" ,  wstatus );

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
    else if ( pid == 0 )
    {
        printf("我是子进程我的ID:%d\n" , getpid() );
        sleep(30);
        return 213 ; // 任何进程的返回值（退出进程） 必须是 0 - 255 之间
    }
    
  
    return 0;
}
