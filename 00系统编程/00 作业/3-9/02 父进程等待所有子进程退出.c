#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>



void ChildProcess( void )
{

    printf("我是子进程%d , 父进程ID：%d\n" , getpid() , getppid()  );
    sleep(3);

    exit(0);
}


int main(int argc, char const *argv[])
{

    // 创建子进程
    pid_t pid;
    for (int i = 0; i < 10; i++)
    {
        pid = fork( );
        if ( pid == 0 )
        {
            break ;
        }
    }
    
    if (pid == 0 )
    {
       ChildProcess();
    }



    for (int i = 0; i < 10 ; i++)
    {
        if(wait(NULL) > 0 )
        {
            printf("成功等待到一个子进程..\n");
        }
    }
    


    return 0;
}
