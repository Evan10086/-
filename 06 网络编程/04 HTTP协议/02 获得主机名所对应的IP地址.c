////////////////////////////////////////////////////////
//
//  Copyright(C), 2005-2022, GEC Tech. Co., Ltd.
//  www.yueqian.com.cn  ALL RIGHT RESERVED
//
//  文件: gethostbyname.c
//  描述: 获取指定域名的IP地址
//
///////////////////////////////////////////////////////
#include <stdio.h>
#include <stddef.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv)
{
    struct hostent *p;
    p = gethostbyname(argv[1]);

    // 列出指定域名的IP地址（列表）
    for(int i=0; p->h_addr_list[i] != NULL; i++)
        printf("%s\n", inet_ntoa(*(struct in_addr*)((p->h_addr_list)[i])));
}