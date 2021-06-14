/*************************************************************************
	> File Name: server.c
	> Author: 少年宇
	> Mail: 
	> Created Time: 2021年06月14日 星期一 11时08分19秒
 ************************************************************************/

#include "../common/head.h"
#include "../common/common.h"
#include "../common/tcp_server.h"
#include "../common/modbus.h"


//#define MAX_N 3
char *conf = "./server.conf";
//四种寄存器
int coils[MAX_N + 5] = {1, 0, 1, 0};
int discrete_inputs[MAX_N + 5];
int holding_regs[MAX_N + 5];
int input_regs[MAX_N + 5];



void output_reg() {
    printf("下面是本机四种寄存器的状态：\n");

    printf("coils : ");
    for (int i = 0; i < 10; i++) printf(" ");
    printf("     %d     %d     %d\n", coils[0], coils[1], coils[2]);

    printf("discrete_inputs : ");
    printf("     %d     %d     %d\n", discrete_inputs[0], discrete_inputs[1], discrete_inputs[2]);

    printf("holding_regs : ");
    for (int i = 0 ; i < 3; i++) printf(" ");
    printf("     %d     %d     %d\n", holding_regs[0], holding_regs[1], holding_regs[2]);


    printf("input_regs : ");
    for (int i = 0; i < 5; i++) printf(" ");
    printf("     %d     %d     %d\n", input_regs[0], input_regs[1], input_regs[2]);

    return ;
}

struct ReadResponseMsg read_data(struct RequestMsg msg) {
    struct ReadResponseMsg rmsg;
    rmsg.slave_addr = 1;//后续要改
    rmsg.cnt = msg.data;
    switch (msg.func_code) {
        case 1: {
            for (int i = msg.data_addr, j = 0; j < msg.data; j++, i++) {
                rmsg.num[j] = coils[i];
            }
        } break;
        case 2:
        case 3: {
            for (int i = msg.data_addr, j = 0; j < msg.data; j++, i++) {
                rmsg.num[j] = holding_regs[i];
            }
        } break;
    }
    return rmsg;
}

void process_read_coils(int fd, struct RequestMsg msg) {
    struct ReadResponseMsg rmsg = read_data(msg);
    rmsg.func_code = 1;
    send(fd, (void *)&rmsg, sizeof(rmsg), 0);
    return ;
}

void process_read_holdingregs(int fd, struct RequestMsg msg) {
    struct ReadResponseMsg rmsg = read_data(msg);
    rmsg.func_code = 3;
    send(fd, (void *)&rmsg, sizeof(rmsg), 0);
    return ;
}


void bit_changed(int *bit, int data) {
    *bit = data;
    return ;
}

void process_write_a_coil(int fd, struct RequestMsg msg) {
    bit_changed(&coils[msg.data_addr], msg.data);
    return ;
}


void process_write_a_holdingreg(int fd, struct RequestMsg msg) {
    bit_changed(&holding_regs[msg.data_addr], msg.data);
    return ;
}

//核心--处理request函数
void process_request(int fd, struct RequestMsg msg) {
    switch (msg.func_code) {
        case 1: {
            process_read_coils(fd, msg);
        } break;
        case 2:
        case 3: {
            process_read_holdingregs(fd, msg);
        } break;
        case 4:
        case 5: {
            process_write_a_coil(fd, msg);
        } break;
        case 6: {
            process_write_a_holdingreg(fd, msg);
        } break;
        case 7:
        case 8:
            break;
        default: break;
    }
    return ;
}



int main() {
    int port, server_listen, sockfd;
    struct RequestMsg msg;
    port = atoi(get_value(conf, "SERVER_PORT"));

    //创建监听状态的socket
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        return 2;
    }

    
    printf("Socket before accept.\n");
    output_reg();
    if ((sockfd = accept(server_listen, 0, 0)) < 0) {
        perror("accept");
        close(sockfd);
    }
    printf("Socket after accept.\n");


    //while循环－－处理request并返回response
    while (1) {
        //收消息
        msg = request_recv(sockfd);
        printf("Request received\n");

        //处理request并返回response
        process_request(sockfd, msg);

        //输出各个寄存器的情况
        output_reg();
    }


    close(sockfd);
    return 0;
}
