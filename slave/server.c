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
#include "../common/color.h"


//#define MAX_N 3
char *conf = "./server.conf";
//四种寄存器
int coils[MAX_N + 5] = {1, 0, 1};
int discrete_inputs[MAX_N + 5] = {0, 1, 1};
int holding_regs[MAX_N + 5] = {1, 1, 0};
int input_regs[MAX_N + 5] = {0, 0, 1};



void output_reg() {
    printf(BLUE_HL("下面是本机四种寄存器的状态：\n"));

    printf(YELLOW_HL("coils : "));
    for (int i = 0; i < 10; i++) printf(" ");
    printf("     %d     %d     %d\n", coils[0], coils[1], coils[2]);

    printf(YELLOW_HL("discrete_inputs : "));
    printf("     %d     %d     %d\n", discrete_inputs[0], discrete_inputs[1], discrete_inputs[2]);

    printf(YELLOW_HL("holding_regs : "));
    for (int i = 0 ; i < 3; i++) printf(" ");
    printf("     %d     %d     %d\n", holding_regs[0], holding_regs[1], holding_regs[2]);


    printf(YELLOW_HL("input_regs : "));
    for (int i = 0; i < 5; i++) printf(" ");
    printf("     %d     %d     %d\n", input_regs[0], input_regs[1], input_regs[2]);

    return ;
}

struct ReadResponseMsg read_data(struct RequestMsg request) {
    struct ReadResponseMsg response;
    response.slave_addr = 1;//后续要改
    response.cnt = request.data;
    switch (request.func_code) {
        case 1: {
            for (int i = request.data_addr, j = 0; j < request.data; j++, i++) {
                response.num[j] = coils[i];
            }
        } break;
        case 2: {
            for (int i = request.data_addr, j = 0; j < request.data; j++, i++) {
                response.num[j] = discrete_inputs[i];
            }
        } break;
        case 3: {
            for (int i = request.data_addr, j = 0; j < request.data; j++, i++) {
                response.num[j] = holding_regs[i];
            }
        } break;
        case 4: {
            for (int i = request.data_addr, j = 0; j < request.data; j++, i++) {
                response.num[j] = input_regs[i];
            }
        } break;
        default:
            break;
    }
    return response;
}

//功能码1
void process_read_coils(int fd, struct RequestMsg request) {
    struct ReadResponseMsg response = read_data(request);
    response.func_code = 1;
    send(fd, (void *)&response, sizeof(response), 0);
    return ;
}

//功能码2
void process_read_discrete_inputs(int fd, struct RequestMsg request) {
    struct ReadResponseMsg response = read_data(request);
    response.func_code = 2;
    send(fd, (void *)&response, sizeof(response), 0);
    return ;
}

//功能码3
void process_read_holdingregs(int fd, struct RequestMsg request) {
    struct ReadResponseMsg response = read_data(request);
    response.func_code = 3;
    send(fd, (void *)&response, sizeof(response), 0);
    return ;
}

//功能码4
void process_read_inputregs(int fd, struct RequestMsg request) {
    struct ReadResponseMsg response = read_data(request);
    response.func_code = 4;
    send(fd, (void *)&response, sizeof(response), 0);
    return ;
}

void bit_changed(int *bit, int data) {
    *bit = data;
    return ;
}

//功能码5
void process_write_a_coil(int fd, struct RequestMsg request) {
    bit_changed(&coils[request.data_addr], request.data);
    send(fd, (void *)&request, sizeof(request), 0);
    return ;
}

//功能码6
void process_write_a_holdingreg(int fd, struct RequestMsg request) {
    bit_changed(&holding_regs[request.data_addr], request.data);
    send(fd, (void *)&request, sizeof(request), 0);
    return ;
}

//核心--处理request函数
void process_request(int fd, struct RequestMsg request) {
    switch (request.func_code) {
        case 1: {
            process_read_coils(fd, request);
        } break;
        case 2: {
            process_read_discrete_inputs(fd, request);
        } break;
        case 3: {
            process_read_holdingregs(fd, request);
        } break;
        case 4: {
            process_read_inputregs(fd, request);
        } break;
        case 5: {
            process_write_a_coil(fd, request);
        } break;
        case 6: {
            process_write_a_holdingreg(fd, request);
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
    struct RequestMsg request;
    port = atoi(get_value(conf, "SERVER_PORT"));

    //创建监听状态的socket
    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        return 2;
    }

    
    output_reg();

    //连接
    bool flag = false;//表示是否通过安全验证
    while (flag == false) {
        if ((sockfd = accept(server_listen, 0, 0)) < 0) {
            perror("accept");
            close(sockfd);
        }
        int password;
        recv(sockfd, (void *)&password, sizeof(password), 0);
        flag = (password == PASSWORD ? true : false);
    }



    //while循环－－处理request并返回response
    while (1) {
        //收消息
        request = request_recv(sockfd);

        //处理request并返回response
        process_request(sockfd, request);

        //输出各个寄存器的情况
        output_reg();
    }


    close(sockfd);
    return 0;
}
