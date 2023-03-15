#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>

#define LOG_FILE "/var/log/custom.log" // 自定义的日志文件路径
#define PIPE_FILE "/tmp/custom.pipe" // 管道文件路径
#define MAX_MSG_LEN 256 // 最大消息长度

static volatile int running = 1; // 守护进程运行标志

// 信号处理函数，用于处理中断信号
static void handle_signal(int signal) {
    running = 0;
}

int main() {
    pid_t pid;
    int fd;
    char buf[MAX_MSG_LEN];
    time_t now;
    struct tm *tm_now;
    struct sigaction sa;

    // 创建管道文件
    if (mkfifo(PIPE_FILE, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    // 创建子进程
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // 在子进程中设置会话ID，以使其成为守护进程
    if (setsid() == -1) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

    // 关闭不需要的文件描述符
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // 打开日志文件
    fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 安装信号处理函数
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_signal;
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // 循环读取管道中的信息并写入日志文件
    while (running) {
        int len;

        // 打开管道文件
        fd = open(PIPE_FILE, O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // 读取管道中的信息
        len = read(fd, buf, MAX_MSG_LEN);
        if (len == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        buf[len] = '\0';

        // 获取当前时间
        now = time(NULL);
        tm_now = localtime(&now);

        // 将信息写入日志文件
        dprintf(fd, "[%d] %04d-%02d-%02d %02d:%02d:%02d %s\n", getpid(),
                tm_now->tm_year + 1900, tm_now->tm_mon + 1, tm_now->tm_mday,
                tm_now->tm_hour, tm_now->tm_min, tm_now->tm_sec, buf);

        // 关闭管道文件
        close(fd);
    }

    // 删除管道文件
    if (unlink(PIPE_FILE)){
        exit(EXIT_SUCCESS);
    }
}


