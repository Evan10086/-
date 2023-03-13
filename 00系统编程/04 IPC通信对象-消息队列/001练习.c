#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

//获取一个key   准备用于消息队列
//把key对应的通信对象设置为消息队列
//阻止好发送的消息内容 （消息类型  消息正文）
//发送消息