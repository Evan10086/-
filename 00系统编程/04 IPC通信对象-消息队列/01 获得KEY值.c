#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>

int main(int argc, char const *argv[])
{
    key_t key1 =  ftok("./",     0x01);
    key_t key2 =  ftok("./",     0x02);
    key_t key3 =  ftok("/home/", 0x01);
    key_t key4 =  ftok("/home/", 0x01);

    printf("key1:%d key2:%d key3:%d  key4:%d \n" , key1 , key2  ,key3 , key4  );
    


    

    return 0;
}
