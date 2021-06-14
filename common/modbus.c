/*************************************************************************
	> File Name: ../common/modbus.c
	> Author: 少年宇
	> Mail: 
	> Created Time: 2021年06月14日 星期一 14时07分00秒
 ************************************************************************/

#include "head.h"
#include "modbus.h"

int request_send(int fd, struct RequestMsg msg) {
    if (send(fd, (void *)&msg, sizeof(msg), 0) <= 0) {
        return -1;
    }

    return 0;
}

struct RequestMsg request_recv(int fd) {
    struct RequestMsg msg;
    if (recv(fd, (void *)&msg, sizeof(msg), 0) <= 0) {
        perror("recv");
    }
    return msg;
}

struct ReadResponseMsg readresponse_recv(int fd) {
    struct ReadResponseMsg rmsg;
    recv(fd, (void *)&rmsg, sizeof(rmsg), 0);

    return rmsg;
}

struct RequestMsg writeresponse_recv(int fd) {
    struct RequestMsg write_response;
    recv(fd, (void *)&write_response, sizeof(write_response), 0);
    return write_response;
}
