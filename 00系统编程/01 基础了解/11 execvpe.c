#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

extern char **environ;

int main(int argc, char const *argv[])
{
    pid_t pid = fork();

    if (pid > 0)
    {
        printf("老莫，我想吃鱼...\n");
    }
    else if (pid == 0 )
    { 
        printf("好的强哥...\n");
        // execl("./test", "./test" , "123" , "456" , "EVEN", NULL );  // 如果加载成功那么当前进程的整个进程空间将会被 test 初始化

        char  * arr[] = {"./test" , "123" , "456" , "EVEN", NULL};
        // char  * env[] = { "PATH=./", "OLDPWD=xxx" , "HOME=/home/Jacy" , NULL};

        char  * env[] = { "PATH=./",  NULL};

        int ret_val = execvpe("./test", arr , env );
        
        perror( "exex error" );
        
    }
    
    printf("[%d]剧情会怎么发展呢？？？？\n" , getpid() );

    return 0;
}
