#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;

int main(int argc, char const *argv[])
{
    pthread_mutex_init(&mutex, NULL );

    // 上锁
    printf("1[%ld]:等待获得访问银行的权限...\n" , pthread_self() );
    pthread_mutex_lock(&mutex);
    printf("1[%ld]:成功获得访问银行的权限...\n" , pthread_self() );

    printf("2[%ld]:等待获得访问银行的权限...\n" , pthread_self() );
    pthread_mutex_lock(&mutex);
    printf("2[%ld]:成功获得访问银行的权限...\n" , pthread_self() );
    


    return 0;
}
