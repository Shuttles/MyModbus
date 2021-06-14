/*************************************************************************
	> File Name: client.c
	> Author: 少年宇
	> Mail: 
	> Created Time: 2021年06月14日 星期一 11时08分30秒
 ************************************************************************/

#include "../common/head.h"
#include "../common/common.h"
#include "../common/tcp_client.h"
#include "../common/modbus.h"

//功能码
//1 read coils
//2 read discrete_inputs
//3 read holding_regs
//4 read input_regs
//5 write a coil
//6 write a holding_reg
//7 write some coils
//8 write some holding_regs
//目前已支持1 3 5 6


char *conf = "./client.conf";
char logfile[50] = {0};
int sockfd;


//处理输入
struct RequestMsg process_input() {
    struct RequestMsg request;
    int func_code = 0;
    printf("请输入功能码\n");
    scanf("%d", &func_code);

    int slave_addr, data_addr, data;
    if (IS_READ(func_code)) {
        printf("请依次输入从机地址、数据地址，读取数据个数：");
    } else {
        printf("请依次输入从机地址、数据地址、要写入的数据：");
    }


    scanf("%d%d%d", &slave_addr, &data_addr, &data);
    request.slave_addr = slave_addr;
    request.func_code = func_code;
    request.data_addr = data_addr;
    request.data = data;
    return request;
}

void process_readresponse(struct RequestMsg request, struct ReadResponseMsg response) {
    if (response.func_code != request.func_code) {
        printf("读操作失败！请重试！\n");
        return ;
    }
    switch (response.func_code) {
        case 1: {
            printf("coils : ");
        } break;
        case 2:
        case 3: {
            printf("Holding_regs : ");
        } break;
        case 4:
        default:
            break;
    }

    for (int i = 0; i < response.cnt; i++) {
        printf("%d ", response.num[i]);
    }
    printf("\n");
    
    return;
}



void process_writeresponse(struct RequestMsg request, struct RequestMsg write_response) {
    if (write_response.func_code != request.func_code) {
        printf("写操作失败！请重试！\n");
    } 
    return ;
}

int main() {
    int port;
    char ip[20] = {0};
    
    //连接前的配置工作
    port = atoi(get_value(conf, "SERVER_PORT"));
    strcpy(ip, get_value(conf, "SERVER_IP"));
    strcpy(logfile, get_value(conf, "LOG_FILE"));


    //连接socket
    if ((sockfd = socket_connect(ip, port)) < 0) {
        perror("socket_connect");
        return 1;
    }
    printf("Socket connected.\n");

    //进行查询或写request
    while (1) {
        //发送request
        struct RequestMsg request = process_input();
        request_send(sockfd, request);
        
        //接收response
        if (IS_READ(request.func_code)) {
            struct ReadResponseMsg read_response = readresponse_recv(sockfd);
            process_readresponse(request, read_response);
        } else {
            struct RequestMsg write_response = writeresponse_recv(sockfd);
            process_writeresponse(request, write_response);
        }
    }



    close(sockfd);
    return 0;
}
