#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void * func(void * arg)
{
    // 设置该线程的取消相应类型为 立即响应
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS , NULL );

    while (1)
    {
        // printf("我在这里摸鱼....\n");
    }

    return NULL ;
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
