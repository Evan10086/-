#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>


       

       
int main(int argc, char const *argv[])
{

    // 获得key 值
    key_t key_shm =  ftok("./", 0x01 );
    if (-1 == key_shm )
    {
        perror("ftok ");
        exit(0);
    }
    perror("ftok ");

    


    // 获得 共享内存的ID 
    int id_shm = shmget(key_shm , 4096 , IPC_CREAT | 0666 );
    if (id_shm == -1 )
    {
        perror("shm get ");
        return -1 ;
    }   
    perror("shm get ");

    
    // 映射到本进程
    char * map = shmat(id_shm , NULL , 0 );
    if (map == (char *)-1 )
    {
        perror("shm at ");
        exit(0);
    }
    perror("shm at ");
    



    // 为所欲为
    while(1)
    {
        fgets(map , 4096 , stdin);
    }


    // 解除映射
    shmdt(map);


    // 删除共享内存 ?? 


    

    return 0;
}
