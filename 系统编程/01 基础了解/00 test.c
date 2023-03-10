#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{

    
    printf("%d__%s__%s\n" , __LINE__ , __FUNCTION__ , __FILE__ );
    
    printf("我的ID：%d , 我收到的参数：%d个 \n" , getpid() , argc  );
    printf( "PATH:%s\n"  , getenv("PATH") );

    for (int i = 0; i < argc ; i++)
    {
        printf("argv[%d]:%s\n" , i , argv[i]);
    }
    

    return 0;
}
