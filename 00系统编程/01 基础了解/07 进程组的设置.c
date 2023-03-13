#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void showID( void )
{
    printf("我是[%d]我处于[%d]进程组..\n" , getpid()  ,getpgid( getpid() ));
    return ;
}

void ParentProcess(pid_t pid)
{

    showID(  );

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
