#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void function (int arg)
{
    printf("[%d]里是响应函数开始....\n" , arg );
    sleep(3);
    printf("[%d]里是响应函数结束....\n" , arg );

}

int main(int argc, char const *argv[])
{
    printf("我的ID：%d\n" , getpid() );
    // 设置当前捕获SIGINT 信号， 并设置他的响应动作是 执行 function 函数
    signal( 34 , function );
    signal( 35 , function );
    signal( 36 , function );

    
    while(1)
    {
        pause();
    }


    return 0;
}
