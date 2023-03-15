#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>

typedef struct {
    int data;
} shared_data;

int main() {
    // 创建共享内
void receive_data(int shmid, int semid) {
    struct data *pdata = shmat(shmid, NULL, 0);
    if (pdata == (void*)-1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }

    struct sembuf semops[1] = { {SEM_RECEIVE, -1, 0} };
    if (semop(semid, semops, 1) == -1) {
        perror("semop failed");
        exit(EXIT_FAILURE);
    }

    printf("Process 2 received data: %s\n", pdata->buffer);

    semops[0].sem_num = SEM_SEND;
    semops[0].sem_op = 1;
    if (semop(semid, semops, 1) == -1) {
        perror("semop failed");
        exit(EXIT_FAILURE);
    }

    if (shmdt(pdata) == -1) {
        perror("shmdt failed");
        exit(EXIT_FAILURE);
    }
}
