#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char const *argv[])
{

    // 获得指定文件描述符的当前的属性信息
    int status = fcntl(0 , F_GETFL );

    // 把属性信息添加上 非阻塞选项
    status |= O_NONBLOCK ;

    // 把添加了科技的属性变量设置回去
    fcntl(0 , F_SETFL , status );

    char ch = getchar(); // 从标准输入中读取一个字符（阻塞型IO）
    printf("ch:%d\n" , ch );
    perror("getchar");

    return 0;
}
