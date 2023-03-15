#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void showid()
{
    printf("本进程ID:%d\t", getpid());
    printf("父进程ID:%d\t", getppid());
    printf("进程组ID:%d\t", getpgid(0));
    printf("所在会话ID:%d\n", getsid(0));
}

int main()
{
    if(fork() == 0)
    {
        showid();
        
        // 将本进程的所属进程组，设置为等于本进程ID
        // 即：创建一个仅包含自身的进程组并将自身置入其中
        if(setpgid(0, 0) < 0)
            perror("setpgid() failed");
        else
            showid();
    }
    
    pause();
}