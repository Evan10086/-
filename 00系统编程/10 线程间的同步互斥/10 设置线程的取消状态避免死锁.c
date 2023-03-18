#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


void * func(void * arg)
{
    
    // 设置取消状态为 使能状态 （不响应取消请求）
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE , NULL );
    // 从此如果收到取消请求，则会暂时挂起 不去响应

    // 【上锁】

    printf("我在这里摸鱼....\n");
    sleep(5);


    // 【解锁】
    
    // 设置取消状态为  使能
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE , NULL );
    // 则程序可以响应已经到达的取消请求

    return NULL ;
}

int main(int argc, char const *argv[])
{
    pthread_t tid ;
    pthread_create(&tid , NULL , func , NULL );
    
    sleep(3);

    pthread_cancel(tid);
    printf("成功发送取消请求。。。\n");

    if(!pthread_join( tid , NULL ))
    {
        printf("成功结合到线程退出...\n");
    }
    

    return 0;
}
