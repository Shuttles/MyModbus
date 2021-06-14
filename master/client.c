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
    struct RequestMsg msg;
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
    msg.slave_addr = slave_addr;
    msg.func_code = func_code;
    msg.data_addr = data_addr;
    msg.data = data;
    return msg;
}

void process_response(struct ReadResponseMsg rmsg) {
    switch (rmsg.func_code) {
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

    for (int i = 0; i < rmsg.cnt; i++) {
        printf("%d ", rmsg.num[i]);
    }
    printf("\n");
    
    return;
}


int main() {
    int port;
    char ip[20] = {0};
    
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
        struct RequestMsg msg = process_input();
        if (request_send(sockfd, msg) != 0) {
            printf("request_send失败！\n");
        } else {
            printf("request_send成功！\n");
        }
        
        //接受response
        if (IS_READ(msg.func_code)) {//如果是写操作就接受response
            struct ReadResponseMsg rmsg = response_recv(sockfd);
            process_response(rmsg);
        }
    }



    close(sockfd);
    return 0;
}
