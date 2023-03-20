#include <stdio.h>

int main(int argc, char const *argv[])
{
    
    FILE * fp =  popen("date" , "r");



    char buf[128] ;
    while(1)
    {
        if(fgets( buf , 128 , fp ))
        {
            printf("buf:%s\n" , buf );

        }

    }
    
    return 0;
}
