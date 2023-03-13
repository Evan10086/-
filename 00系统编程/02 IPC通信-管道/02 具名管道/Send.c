#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define     FIFO_PATH "/tmp/myFIfo"

int main(int argc, char const *argv[])
{

    // 创建管道文件
    if(access(FIFO_PATH , F_OK ))
    {
        if( mkfifo ( FIFO_PATH , 0666 ))
        {
            perror("管道创建失败....");
            return -1 ;
        }
    }

    // 打开管道文件
    int  fd_fifo = open(FIFO_PATH , O_WRONLY ); // 使用只写的方式打开 如果当前没有 读者 则打开会造成阻塞
    if (fd_fifo == -1 )
    {
        perror("open fifo error");
        return -1 ;
    }
    else{
        printf("打开管道文件成功...\n");
    }

    // 发送消息
    char buf [32];
    while(1)
    {
        bzero(buf, 32);

        printf ("请输入需要发送的数据：\n");
        fgets(buf , 32 , stdin);
        int ret_Val = write( fd_fifo , buf , strlen(buf)  );
        printf("成功发送 %d 字节的数据....\n" , ret_Val);

    }


    return 0;
}
