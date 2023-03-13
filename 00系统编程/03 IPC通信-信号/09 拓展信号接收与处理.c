#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>


void func(int sigNum , siginfo_t * info , void * ptr )
{

    printf("这里是特殊信号相应函数...\n");
    printf("当前收到的信号值：%d , 所携带的数据是：%s 。。。\n" , sigNum , (char* )info->si_ptr);
    printf("info: %p\n" , info->si_ptr );
    //   

}


int main(int argc, char const *argv[])
{
    struct sigaction act = {
        .sa_sigaction = func ,  // 设置拓展相应函数的入口地址
        .sa_flags = SA_SIGINFO  // 选择使用 拓展相应函数，非标准处理
    };

    printf("pid:%d\n",  getpid() );

    sigaction( 45 , &act , NULL );

    while (1)
    {
        pause();
    }
    
    
    return 0;
}
