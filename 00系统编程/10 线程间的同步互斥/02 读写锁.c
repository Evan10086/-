#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_rwlock_t rwlock ;

char buf [32] = {"Hello GEC"} ;   

void * func (void * arg )
{

    while (1)
    {
        sleep(1);
        printf("[%ld]:正在等待上读锁...\n" , pthread_self());
        pthread_rwlock_rdlock(&rwlock);
        printf("[%ld]:成功获得读锁...\n" , pthread_self());

        printf("buf:%s\n" , buf );
        sleep(1);


        printf("[%ld]:释放读锁资源...\n" , pthread_self());
        pthread_rwlock_unlock(&rwlock);
    }
    


}

int main(int argc, char const *argv[])
{

    // 初始化读写锁
    pthread_rwlock_init(&rwlock , NULL );

    pthread_t thread;
    for (int i = 0; i < 5; i++)
    {
        pthread_create(&thread , NULL ,func , NULL);
    }
    

    while(1)
    {
        printf("主函数正在等待上写锁....\n");
        pthread_rwlock_wrlock(&rwlock);
        printf("主函数成功获得写锁....\n");

        fgets(buf , 32 , stdin);

        printf("主函数释放写锁资源...\n" );
        pthread_rwlock_unlock(&rwlock);

    }
    
    return 0;
}
