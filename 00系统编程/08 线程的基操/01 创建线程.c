#include <stdio.h>
#include <pthread.h>
#include <errno.h>

int Num = 1 ;

void * Func (void * arg)
{

    while(1)
    {
        if (Num %2)
        {

            printf("%d - %s这里是子函数,收到的参数：%s.... \n" , __LINE__ , __FUNCTION__  , (char *)arg );
            Num ++ ;
        }
        
    }

}

int main(int argc, char const *argv[])
{
    pthread_t thread ;
    
    char * msg = "Hello GZ2301....";

    
    if(errno =  pthread_create(&thread , NULL , Func , msg ) )
    {
        perror("pthread create ");
        return -1 ;
    }

    while(1)
    {
        if ( !(Num %2) )
        {
            printf("%d - %s这里是主函数.... \n" , __LINE__ , __FUNCTION__ );
            Num ++ ;
        }
    }


    
    return 0;
}
