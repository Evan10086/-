#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void routine(void * arg)
{
    printf("这里是取消处理函数:%d\n" , (int)arg);
}

void * func(void * arg)
{

    pthread_cleanup_push(routine , (void *)250 );
    pthread_cleanup_push(routine , (void *)251 );
    pthread_cleanup_push(routine , (void *)252);
    

    // while (1)
    {
        printf("我在这里摸鱼....\n");
        sleep(1); 
        pthread_cleanup_pop( 1 ); //  【错误】 压栈与弹栈的操作必须成对出现在同一层的代码块中
    }
    
    pthread_cleanup_pop( 0 ); // 弹出压栈函数并不执行他
    pthread_cleanup_pop( 100 );
}

int main(int argc, char const *argv[])
{
    pthread_t tid ;
    pthread_create(&tid , NULL , func , NULL );
    
    sleep(3);

    pthread_cancel(tid);

    if(!pthread_join( tid , NULL ))
    {
        printf("成功结合到线程退出...\n");
    }
    

    return 0;
}
