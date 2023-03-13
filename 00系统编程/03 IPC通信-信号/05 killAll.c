#include <stdio.h>
#include <signal.h>
#include <stdlib.h>


int main(int argc, char const *argv[])
{
    
    for (int i = 1 ; i <= 64; i++)
    {
        if (i == 9 || i == 19 || i == 32 || i == 33 )
        {
            continue; 
        }
        kill( atoi(argv[1]) , i );
        printf("发送信号%d给%d\n" , i , atoi(argv[1]));
    }
    
    

    return 0;
}
