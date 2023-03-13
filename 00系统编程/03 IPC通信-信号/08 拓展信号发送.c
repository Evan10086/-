#include <stdio.h>
#include <signal.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
    
    union sigval info = {
        .sival_int = 123 ,
        .sival_ptr = "Hello GZ2301"
    };


    sigqueue( atoi(argv[1]) , 45 , info ); // 产生信号 45 号 ，并携带整型数据 123 传递给对方

        printf("info: %p\n" , info.sival_ptr );

    
    return 0;
}
