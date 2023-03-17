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

    pthread_exit( msg );
    // return arr ;
    // exit(0);

}

int main(int argc, char const *argv[])
{

    pthread_attr_t attr ; // 定义一个属性变量

    pthread_attr_init(&attr); // 初始化属性变量

    pthread_attr_setdetachstate(&attr , PTHREAD_CREATE_DETACHED ); // 给属性变量添加分离的选项

    pthread_t thread ;
    // 使用填了了分离科技的变量来创建线程
    pthread_create(&thread, &attr ,FUNC, NULL );

    


    char * ptr ;
    if( errno = pthread_join( thread, (void **)&ptr ))
    {
        perror("join error");
        return -1 ;
    }

    printf("成功接合：%s\n" , ptr );


    return 0;
}
