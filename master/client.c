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



char *conf = "./client.conf";
char logfile[50] = {0};
int sockfd;


struct ReadMsg process_input() {
    struct ReadMsg msg;
    int func_num = 0;
    printf("请输入功能码\n");
    scanf("%d", &func_num);
    if (func_num >= 1 && func_num <= 4) {
        int slave_addr, data_addr, data_cnt;
        printf("请依次输入从机地址、数据地址，读取数据个数：");
        scanf("%d%d%d", &slave_addr, &data_addr, &data_cnt);
        msg.slave_addr = slave_addr;
        msg.func_num = func_num;
        msg.data_addr = data_addr;
        msg.data_cnt = data_cnt;
    } else {

    }
    
    return msg;
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
        struct ReadMsg msg = process_input();
        if (request_send(sockfd, msg) != 0) {
            printf("request_send失败！\n");
        } else {
            printf("request_send成功！\n");
        }
        
        //接受response
        struct ReadResponseMsg rmsg = response_recv(sockfd);
        for (int i = 0; i < rmsg.cnt; i++) {
            printf("%d ", rmsg.num[i]);
        }
        printf("\n");
    }



    close(sockfd);
    return 0;
}
