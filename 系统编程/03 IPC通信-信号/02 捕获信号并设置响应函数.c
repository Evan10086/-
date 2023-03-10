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
    
    while(1)
    {
        printf("我的ID：%d\n" , getpid() );
        sleep(1);
    }


    return 0;
}
