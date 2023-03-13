#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    pid_t pid = fork();
    if (pid == -1 )
    {
        perror("fork error");
        exit(-1); 
    }

    if (pid > 0 )
    {
        while(1)
        {
            printf("我是父进程，我的ID是：%d , 我有个孩子，它的ID：%d \n" , getpid() , pid );
        }
    }
    else if (pid == 0)
    {
        while(1)
        {
            printf("我是子进程，我的ID是：%d  \n" , getpid()  );
        }
    }
    
    
    
    
    return 0;
}
