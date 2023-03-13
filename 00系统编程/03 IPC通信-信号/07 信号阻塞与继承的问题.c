#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void function (int arg)
{
    printf("[%d][%d]这里是响应函数....\n" , getpid()  , arg );
}

void ChildProcess( sigset_t set )
{

    printf("[%d]这里是子进程...\n" , getpid () );

    // 解除处于set信号集中的信号
    // sigprocmask( SIG_UNBLOCK , &set, NULL );

    while(1)
    {
        pause() ;
    }

}

int main(int argc, char const *argv[])
{
    printf("我的ID：%d\n" , getpid() );

    // 设置当前捕获SIGINT 信号， 并设置他的响应动作是 执行 function 函数
    for (int i = 1 ; i <= 64; i++)
    {
        if (i == 9 || i == 19 || i == 32 || i == 33 )
        {
            continue; 
        }
        signal( i , function );
    }
    
    

    // 创建信号集
    sigset_t set;

    // 清空信号集 
    sigemptyset(&set); 

    // 把需要设置阻塞的信号添加到信号集中 (Cntl + c 所产生的信号 --> SIGINT)
    sigfillset(&set);

    // 设置阻塞信号集中的信号
    sigprocmask( SIG_SETMASK , &set, NULL );


    // 从以以后所有  SIGINT 都会被暂时挂起，不去响应他.....
    sleep(10);


    pid_t pid = fork();
    if (pid == 0 )
    {
        ChildProcess(set);
    }

    sleep(10);

    

    // 解除处于set信号集中的信号
    sigprocmask( SIG_UNBLOCK , &set, NULL );
    // 从以以后所有  SIGINT 都会正常响应....


    while (1)
    {
        // printf("我的ID：%d\n" , getpid() );
        // sleep(1);
        pause();
    }
    

    return 0;
}
