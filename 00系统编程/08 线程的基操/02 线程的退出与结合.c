#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

       
void * FUNC (void * arg )
{

    char * msg = "Hello Even";
    static char arr [] = "Hello Even" ;

    printf("%d-%s-%s\n" , __LINE__ , __FUNCTION__ , __FILE__ );

    sleep(3);

    pthread_exit( arr );
    // return arr ;
    // exit(0);

}

int main(int argc, char const *argv[])
{
    
    pthread_t thread ;
    pthread_create(&thread, NULL ,FUNC, NULL );
    sleep(1);
    
    pthread_cancel( thread );
    usleep(100) ;
    char * ptr ;
    if( errno = pthread_tryjoin_np( thread, (void **)&ptr ))
    {
        perror("join error");
        return -1 ;
    }

    printf("成功接合：%s\n" , ptr );


    return 0;
}
