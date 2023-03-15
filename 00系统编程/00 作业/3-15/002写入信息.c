#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define PIPE_FILE "/tmp/custom.pipe" // 管道文件路径
#define MAX_MSG_LEN 256 // 最大消息长度

int main() {
    int fd;
    char buf[MAX_MSG_LEN];
    pid_t pid = getpid();

    // 打开管道文件
    fd = open(PIPE_FILE, O_WRONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // 写入信息到管道中
    snprintf(buf, MAX_MSG_LEN, "错误信息");
    write(fd, buf, strlen(buf));
    close(fd);

    return 0;
}
