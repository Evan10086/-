#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void fucn(int sigNum)
{

    printf("闹钟响了.. %d\n" , sigNum );

    alarm( 5 );
}

int main(int argc, char const *argv[])
{   
    // 设置捕获闹钟信号
    signal(SIGALRM , fucn );


    alarm( 10 );

    sleep(3);

    int ret_val = alarm( 5 );
    printf("ret_val:%d\n" , ret_val );


    while(1)
    {
        pause();
    }

    
    return 0;
}
