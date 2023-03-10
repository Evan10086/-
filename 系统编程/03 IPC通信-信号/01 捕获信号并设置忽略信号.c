#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(int argc, char const *argv[])
{
    
    signal( SIGINT , SIG_IGN );
    
    while(1)
    {
        printf("我的ID：%d\n" , getpid() );
        sleep(1);
    }


    return 0;
}
