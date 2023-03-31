#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "120.78.234.61"       // 定义服务器IP地址
#define SERVER_PORT 8000            // 定义服务器监听的端口号
#define MAX_BUFFER_SIZE 1024        // 定义缓冲区大小

int main(int argc, char *argv[])
{
struct sockaddr_in server_addr; // 定义服务器地址结构体
int client_socket_fd;           // 定义客户端套接字文件描述符
char buffer[MAX_BUFFER_SIZE];   // 定义缓冲区

// 创建客户端套接字
if ((client_socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(1);
}

// 设置服务器地址结构体
memset(&server_addr, 0, sizeof(server_addr));   // 初始化为0
server_addr.sin_family = AF_INET;              // IPv4地址
server_addr.sin_port = htons(SERVER_PORT);      // 设置服务器监听的端口号
inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);  // 将服务器IP地址转换为网络字节序

// 向服务器发送消息
strcpy(buffer, "Hello, server!");
if (sendto(client_socket_fd, buffer, strlen(buffer), 0,
(struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
perror("sendto");
exit(1);
}

// 接收服务器响应
memset(buffer, 0, MAX_BUFFER_SIZE);   // 清空缓冲区
socklen_t addr_len = sizeof(server_addr);
if (recvfrom(client_socket_fd, buffer, MAX_BUFFER_SIZE, 0,
(struct sockaddr *)&server_addr, &addr_len) == -1) {
perror("recvfrom");
exit(1);
}

printf("Server response: %s\n", buffer);

close(client_socket_fd);   // 关闭客户端套接字
return 0;
}