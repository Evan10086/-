#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8000                // 定义服务器监听的端口号
#define MAX_BUFFER_SIZE 1024            // 定义缓冲区大小

int main(int argc, char *argv[])
{
    struct sockaddr_in server_addr;     // 定义服务器地址结构体
    int server_socket_fd;               // 定义服务器套接字文件描述符
    char buffer[MAX_BUFFER_SIZE];       // 定义缓冲区

    // 创建服务器套接字
    if ((server_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // 设置服务器地址结构体
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 绑定服务器套接字到指定地址和端口
    if (bind(server_socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    printf("服务器已启动，等待客户端连接...\n");

    while (1) {

        struct sockaddr_in client_addr;                     // 定义客户端地址结构体
        socklen_t client_addr_len = sizeof(client_addr);   
        
        // 接收客户端请求
        int recv_len = recvfrom(server_socket_fd, buffer, MAX_BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (recv_len == -1) {
            perror("recvfrom");
            continue;
        }

        printf("接收到来自客户端 %s:%d 的消息：%s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

        // 处理客户端请求
        // ...

        // 发送响应消息
        int send_len = sendto(server_socket_fd, "hello, client!", strlen("hello, client!"), 0, (struct sockaddr *)&client_addr, client_addr_len);
        if (send_len == -1) {
            perror("sendto");
            continue;
        }

        printf("已向客户端 %s:%d 发送响应消息\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

// 关闭服务器套接字
close(server_socket_fd);

return 0;
}