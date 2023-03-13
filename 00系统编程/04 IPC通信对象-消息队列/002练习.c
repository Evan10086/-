#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
// 获得一个KEY值 准备用于消息队列
// 把key 随对应的通信对象设置为消息队列
// 阻止好发送的消息内容  （消息类型 + 消息正文）
// 发送消息