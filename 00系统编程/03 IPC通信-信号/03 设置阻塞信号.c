#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void function (int arg)
{
    printf("这里是响应函数....\n");
}

int main(int argc, char const *argv[])
{
    // 设置当前捕获SIGINT 信号， 并设置他的响应动作是 执行 function 函数
    signal( SIGINT , function );
    

    // 创建信号集
    sigset_t set;

    // 清空信号集 
    sigemptyset(&set); 

    // 把需要设置阻塞的信号添加到信号集中 (Cntl + c 所产生的信号 --> SIGINT)
    sigaddset(&set, SIGINT );

    // 设置阻塞信号集中的信号
    sigprocmask( SIG_SETMASK , &set, NULL );

    // 从以以后所有  SIGINT 都会被暂时挂起，不去响应他.....
    sleep(5);

    // 解除处于set信号集中的信号
    sigprocmask( SIG_UNBLOCK , &set, NULL );
    // 从以以后所有  SIGINT 都会正常响应....


    while (1)
    {
        printf("我的ID：%d\n" , getpid() );
        sleep(1);
    }
    

    return 0;
}
