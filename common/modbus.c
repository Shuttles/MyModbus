/*************************************************************************
	> File Name: ../common/modbus.c
	> Author: 少年宇
	> Mail: 
	> Created Time: 2021年06月14日 星期一 14时07分00秒
 ************************************************************************/

#include "head.h"
#include "modbus.h"

int request_send(int fd, struct ReadMsg msg) {
    if (msg.func_num >= 1 && msg.func_num <= 4) {
        //read
        if (send(fd, (void *)&msg, sizeof(msg), 0) <= 0) {
            return -1;
        }

    } else {
        //write
    }


    return 0;
}

struct ReadMsg request_recv(int fd) {
    struct ReadMsg msg;
    if (recv(fd, (void *)&msg, sizeof(msg), 0) <= 0) {
        perror("recv");
    }
    return msg;
}

struct ReadResponseMsg response_recv(int fd) {
    struct ReadResponseMsg rmsg;
    recv(fd, (void *)&rmsg, sizeof(rmsg), 0);

    return rmsg;
}
