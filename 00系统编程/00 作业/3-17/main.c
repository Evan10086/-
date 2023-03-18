#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "list.h"

pthread_rwlock_t rwlock ;


void * removeNode( void * arg )
{
    P_Node head = (P_Node)arg ;

    // 申请上写锁
    printf("[%ld]：正在等待上写锁，并准备剔除某一个结点...\n" , pthread_self() );
    pthread_rwlock_wrlock(&rwlock);
    printf("[%ld]：成功上写锁，剔除某一个结点...\n" , pthread_self() );

    
    DelNode ( head , head->Next  , head->Next->Next ) ;
    sleep(2);

    // 解开写锁
    printf("[%ld]:剔除结点完成正要解锁...\n" , pthread_self() );
    pthread_rwlock_unlock(&rwlock);
}

void * showNode( void * arg )
{
    P_Node head = (P_Node)arg;

    // 申请上读锁
    printf("[%ld]：正在等待上读锁，并准备遍历整个链表...\n" , pthread_self() );
    pthread_rwlock_rdlock(&rwlock);
    printf("[%ld]：成功上读锁，遍历整个链表...\n" , pthread_self() );


    // 遍历访问链表
    DisplayList( head , 0 );
    sleep(2);

    printf("[%ld]：正在解锁...\n" , pthread_self() );
    pthread_rwlock_unlock(&rwlock);

}

int main(int argc, char const *argv[])
{
    // 链表的初始化
    P_Node head = NewNode( NULL );

    // 初始化读写锁
    pthread_rwlock_init( &rwlock, NULL );
    
    printf("[%ld]：正在等待上写锁，并准备添加一些结点...\n" , pthread_self() );
    pthread_rwlock_wrlock(&rwlock);
    printf("[%ld]：成功上写锁，正在添加结点...\n" , pthread_self() );


    // 创建线程
    pthread_t thread [6];
    int i ;
    for ( i = 0; i < 3 ; i++)
    {
        /* 剔除链表结点的线程 */
        pthread_create(&thread[i], NULL ,removeNode , (void *)head);
    }

    for (i = 3; i < 6 ; i++)
    {
        /* 遍历显示链表结点的线程 */
        pthread_create(&thread[i], NULL ,showNode , (void *)head);
    }

    /* 主函数通过用户手动创建新节点并插入链表中 */
    for (int i = 0; i < 10; i++)
    {
        DataType Data;
        scanf("%d" , &Data);
        P_Node New = NewNode( &Data );

        Add2List( head , New , head->Next ); // 在链表头部插入

    }

    printf("[%ld]：正在解锁...\n" , pthread_self() );
    pthread_rwlock_unlock(&rwlock);

    for (int i = 0; i < 6; i++)
    {
        pthread_join(thread[i] , NULL);
        printf("成功结合线程%ld\n" , thread[i]);
    }
    

    return 0;
}
