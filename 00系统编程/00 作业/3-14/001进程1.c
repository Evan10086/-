#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define SHM_SIZE 1024

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

struct shared_memory {
    int data;
};

int main() {
    int shmid, semid;
    key_t key;
    struct shared_memory *shm_ptr;
    union semun sem_init_val;

    // 获取共享内存键值
    key = ftok("/tmp", 'A');

    // 创建共享内存
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget error");
        exit(1);
    }

    // 映射共享内存
    shm_ptr = (struct shared_memory*) shmat(shmid, NULL, 0);
    if (shm_ptr == (void*) -1) {
        perror("shmat error");
        exit(1);
    }

    // 创建信号量
    semid = semget(key, 2, IPC_CREAT | 0666);
    if (semid < 0) {
        perror("semget error");
        exit(1);
    }

    // 初始化信号量
    sem_init_val.val = 0;
    semctl(semid, 0, SETVAL, sem_init_val);
    sem_init_val.val = 1;
    semctl(semid, 1, SETVAL, sem_init_val);

    // 发送数据
    while (1) {
        // 等待进程2接收数据的信号量
        struct sembuf sem_op;
        sem_op.sem_num = 1;
        sem_op.sem_op = -1;
        sem_op.sem_flg = 0;
        semop(semid, &sem_op, 1);

        // 写入共享内存
        printf("Enter a number: ");
        scanf("%d", &(shm_ptr->data));

        // 释放进程1发送数据的信号量
        sem_op.sem_num = 0;
        sem_op.sem_op = 1;
        sem_op.sem_flg = 0;
        semop(semid, &sem_op, 1);
    }

    // 解除共享内存映射
    shmdt((void*) shm_ptr);

    // 删除共享内存
    shmctl(shmid, IPC_RMID, NULL);

    // 删除信号量
    semctl(semid, 0, IPC_RMID, 0);
    semctl(semid, 1, IPC_RMID, 0);

    return 0;
}
