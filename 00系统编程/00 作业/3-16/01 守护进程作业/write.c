#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define     FIFO_PATH   "/tmp/fifo_demo"
#define     LOG_PATH    "/home/even/test.log"

int toDaemon(void)
{
	pid_t a;
	int max_fd, i;

	/*********************************************
	1. ignore the signal SIGHUP, prevent the
	   process from being killed by the shutdown
	   of the present controlling termination
	**********************************************/
	signal(SIGHUP, SIG_IGN);

	/***************************************
	2. generate a child process, to ensure
	   successfully calling setsid()
	****************************************/
	a = fork();
	if(a > 0)
		exit(0);

	/******************************************************
	3. call setsid(), let the first child process running
	   in a new session without a controlling termination
	*******************************************************/
	setsid();

	/*************************************************
	4. generate the second child process, to ensure
	   that the daemon cannot open a terminal file
	   to become its controlling termination
	**************************************************/
	a = fork();
	if(a > 0)
		exit(0);

	/*********************************************************
	5. detach the daemon from its original process group, to
	   prevent any signal sent to it from being delivered
	**********************************************************/
	setpgrp();
	// setpgid(0 ,0);

	/*************************************************
	6. close any file descriptor to release resource
	**************************************************/
	max_fd = sysconf(_SC_OPEN_MAX);
	for(i=0; i<max_fd; i++)
		close(i);

	/******************************************
	7. clear the file permission mask to zero
	*******************************************/
	umask(0);

	/****************************************
	8. change the process's work directory,
	   to ensure it won't be uninstalled
	*****************************************/
	chdir("/");


	// Congratulations! Now, this process is a DAEMON!
	// 为所欲为调用自己的功能即可
	// 注意所有的路径都是 以 / 根目录为起点  代码中不应该出现相对路径
	// 不能在使用printf / scanf / perror 这一类 通过标准输入 或标准输出 标准出错等的函数
	// 所有需要打印输出或输入的内容，可以通过管道文件，或日志文件进行交互
	// pause();
	return 0;
}

int InitFifo( void )
{
    // 检查目标管道是否存在
    if( access(FIFO_PATH , F_OK | R_OK | W_OK))
    {
        // 如果不存在则创建
        if (access(FIFO_PATH , F_OK ))
        {
            if( mkfifo(FIFO_PATH , 0666))
            {
                perror("mkfifo error");
                exit(0);
            }
        }
        else   // 如果是因为不能读写而导致错误
        {
            /* 修改管道文件的权限 */
            if( chmod(FIFO_PATH , 0666 ))
            {
                perror( "chmod error");
                exit(0);
            }
        }
        
        

    }


    // 打开管道文件
    int fd = open( FIFO_PATH , O_RDWR );
    if (fd == -1 )
    {
        perror("open fifo error");
        exit(0);
    }
    
    printf("管道初始化成功....\n");
    // 返回被打开的文件描述符
    return fd ;
}

FILE * InitLog( void )
{
    FILE * fp = fopen( LOG_PATH , "a+"); // 如果项目中需要打开文件则建议使用绝对路径
	if ( fp == NULL )
	{
		perror("open file error");  // 如果守护进程设置成功，则该语句将不会有任何的执行效果
		exit(0);
	}

    return fp ;
}

int main(int argc, char const *argv[])
{
	// 把当前进程设置为守护进程
	toDaemon() ;


    // 初始化管道文件
    int fd_fifo = InitFifo();

    // 初始化日志文件
    FILE * fp_log = InitLog();


    char * msg = calloc(1, 128);
    time_t timep;
	while(1)
	{

        bzero(msg , 128 );


        // pid#msg....
        // 123#open file error ....
        read( fd_fifo , msg , 128);

        // char * msg_test = "1234#open file error ...."  ;
        int pid = atoi(msg);
        printf("pid:%d\n" , pid );

        char * tmp_p = strchr(msg , '#') + 1 ;
        printf("msg:%s\n" , tmp_p );


        time (&timep); // 获得当前的时间和日期 （是一个秒为单位的数字）
        char * tmp_s = ctime(&timep); // 使用ctime 把 数字转换成一个 字符串并返回
        tmp_s[strlen(tmp_s)-1] = '\0';
        printf("time:%s\n" , tmp_s );

		// Fri Mar 17 09:38:20 2023\n\0
        fprintf(fp_log , "[%d] [%s] %s\n", pid ,tmp_s  , tmp_p );

        fflush(fp_log);

	}
	


	return 0;
}

