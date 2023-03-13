#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

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

        char  * arr[] = {"ls" , "-a" , "-l" , NULL};
        execv("/bin/ls", arr ); //  没有使用 带 p 的版本 ，则需要写可执行文件的完整路径 否则无法加载
        execvp("ls", arr ); // 使用带有 p 的版本， 则（对于命令来说）无需给定完整路径，则会自动从PATH 中搜索指定程序来加载
    }
    
    printf("[%d]剧情会怎么发展呢？？？？\n" , getpid() );

    return 0;
}
