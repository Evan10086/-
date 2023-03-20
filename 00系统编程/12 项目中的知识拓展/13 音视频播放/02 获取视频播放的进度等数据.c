#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void * getMplayerMsg (void * arg)
{   
    FILE * fp = (FILE *)arg ;


    char buf [128] ;

    while(1)
    {
        bzero(buf , 128);
        usleep(2000);

        fgets( buf , 128 , fp  );

        printf("buf :%s \n" , buf );

    }


}


int main(int argc, char const *argv[])
{
    if( access("/tmp/myFifo", F_OK ))
    {
        if(mkfifo("/tmp/myFifo", 0666 ))
        {
            perror("mkfifo error");
            return -1 ;
        }
    }
    
    int fd_fifo = open("/tmp/myFifo" , O_RDWR );
    if (fd_fifo == -1 )
    {
        perror("open fifor error");
        return -1 ;
    }
    
    
    FILE * fp =  popen("mplayer -quiet -slave -input file=/tmp/myFifo -geometry 100:100 -zoom -x 600 -y 300  /Even/video/Faded3.avi &" , "r");
    // system("mplayer -quiet -slave -input file=/tmp/myFifo /Even/video/Faded3.avi &" );// 直接执行该命令 & 指该命令在后台执行

    pthread_t tid ;
    pthread_create( &tid , NULL , getMplayerMsg ,  fp  );


    char * cmd = "get_percent_pos\n" ;

    while(1)
    {
        sleep(3) ;
        write(fd_fifo , cmd  , strlen(cmd ) );
        printf("申请获得时间。。。。。\n");
    }
    
    return 0;
}
