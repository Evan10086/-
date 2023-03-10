#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{

    if(access("/tmp/myFIfo" , F_OK ))
    {
        if( mkfifo ( "/tmp/myFIfo" , 0666 ))
        {
            perror("管道创建失败....");
            return -1 ;
        }
    }


    return 0;
}
