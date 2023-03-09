#include <stdio.h> 
#include <unistd.h> 

int main()
{
    printf("[]fork之前\n");

    pid_t pid = fork();
    // 以上函数执行成功后
    // 父子进程都将从下面的语句开始执行，不分先后

    // 以下语句会被执行两遍
    // 在父进程中，pid将是子进程的PID
    // 在子进程中，pid将是0
    if (pid > 0 )
    {
        sleep(15);
    }
    // if (pid == 0 )
    // {
    //     sleep(10);
    // }
    printf("[%d]: pid=%d\n", getpid(), pid);
    
}