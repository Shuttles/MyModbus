/*************************************************************************
	> File Name: modbus.h
	> Author: 少年宇
	> Mail: 
	> Created Time: 2021年06月14日 星期一 13时55分53秒
 ************************************************************************/

#ifndef _MODBUS_H
#define _MODBUS_H

#define MAX_N 3

struct ReadMsg{
    int slave_addr;
    int func_num;
    int data_addr;
    int data_cnt;
};

struct WriteMsg {
    int slave_addr;
    int func_num;
    int data_addr;
    int data;
};


struct ReadResponseMsg {
    int slave_addr;
    int func_num;
    int cnt;
    int num[MAX_N + 5];
};

int request_send(int, struct ReadMsg);

struct ReadMsg request_recv(int);

struct ReadResponseMsg response_recv(int);

#endif
