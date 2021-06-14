/*************************************************************************
	> File Name: modbus.h
	> Author: 少年宇
	> Mail: 
	> Created Time: 2021年06月14日 星期一 13时55分53秒
 ************************************************************************/

#ifndef _MODBUS_H
#define _MODBUS_H


struct RequestMsg{
    int slave_addr;
    int func_code;
    int data_addr;
    int data;//如果是读请求，则data代表要读取的寄存器的数量；如果是写请求，则data代表要写入的数据
};


struct ReadResponseMsg {
    int slave_addr;
    int func_code;
    int cnt;
    int num[MAX_N + 5];
};

int request_send(int, struct RequestMsg);

struct RequestMsg request_recv(int);

struct ReadResponseMsg readresponse_recv(int);

struct RequestMsg writeresponse_recv(int);

#endif
