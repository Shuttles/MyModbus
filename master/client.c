/*************************************************************************
	> File Name: client.c
	> Author: 少年宇
	> Mail: 
	> Created Time: 2021年06月14日 星期一 11时08分30秒
 ************************************************************************/

#include "../common/head.h"
#include "../common/common.h"
#include "../common/tcp_client.h"

char *conf = "./client.conf";
char logfile[50] = {0};
int sockfd;


int main() {
    int port;
    char ip[20] = {0};
    
    port = atoi(get_value(conf, "SERVER_PORT"));
    strcpy(ip, get_value(conf, "SERVER_IP"));
    strcpy(logfile, get_value(conf, "LOG_FILE"));


    if ((sockfd = socket_connect(ip, port)) < 0) {
        perror("socket_connect");
        return 1;
    }
    printf("Socket connected.\n");

    if (send(sockfd, "chenzheyu", sizeof("chenzheyu"), 0) < 0) {
        perror("send");
        return 2;
    }

    close(sockfd);
    return 0;
}
