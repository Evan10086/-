#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/*
    一下代码实验发现：
        父进程在创建子进程前打开的文件，是会被子进程继承下去。
        父子进程中的文件描述符是指向同一套资源的， 因此他们共享所有的读写操作。
        父子进程之间任何一方关闭文件并不会影响另一方的使用 不共享 CLOSE_ON_EXIT 标记
*/



void ParentProcess( int fd )
{

    char * msg = calloc(1, 128) ;

    while (1)
    {
        bzero(msg , 128);
        printf("请输入新的数据：\n");
        int ret_val =  lseek( fd, 0 , SEEK_CUR);

        fgets(msg , 128 , stdin );

        ret_val = write( fd, msg , strlen(msg) );
        ret_val =  lseek( fd, 0 , SEEK_CUR);

        printf("成功写入%d个字节..\n" , ret_val);

        if ( !strcmp( msg , "byebye\n" ) )
        {
            close(fd);
            break; 
        }
        
    }

    free(msg);

    return ;
}

void ChildProcess( int fd )
{

    char buf [128] ;

    while(1)
    {
        bzero(buf , 128) ;

        int ret_val =  lseek( fd, 0 , SEEK_SET);
        printf("当前读写位置为：%d\n" , ret_val);
        ret_val = read(fd , buf , 128 );
        printf("成功读取到%d个字节：%s\n" , ret_val , buf );

        sleep(2);
    }

}


int main(int argc, char const *argv[])
{

    if ( argc != 2 )
    {
        printf("请传递正确的参数..\n");
        return -1 ;
    }
    

    int fd = open(argv[1] , O_RDWR | O_CREAT , 0666 );
    if (fd == -1 )
    {
        perror("open file error");
        return -1 ;
    }

    // 创建子进程
    pid_t pid = fork( );
    if ( pid > 0 )
    {
        ParentProcess(fd);
    }
    else if (pid == 0 )
    {
       ChildProcess(fd);
    }
    else {
        perror("fork error");
    }

    return 0;
}
