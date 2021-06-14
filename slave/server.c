/*************************************************************************
	> File Name: server.c
	> Author: 少年宇
	> Mail: 
	> Created Time: 2021年06月14日 星期一 11时08分19秒
 ************************************************************************/

#include "../common/head.h"
#include "../common/common.h"
#include "../common/tcp_server.h"

char *conf = "./server.conf";

int main() {
    int port, server_listen, sockfd;
    
    port = atoi(get_value(conf, "SERVER_PORT"));

    if ((server_listen = socket_create(port)) < 0) {
        perror("socket_create");
        return 2;
    }

    while (1) {
        printf("Socket before accept.\n");
        if ((sockfd = accept(server_listen, 0, 0)) < 0) {
            perror("accept");
            close(sockfd);
            continue;
        }

        printf("Socket after accept.\n");

        char name[20] = {0};
        if (recv(sockfd, name, sizeof(name), 0) <= 0) {
            perror("recv");
            close(sockfd);
            continue;
        }
        printf("Socket received\n");
        printf("Name = %s\n\n", name);
        close(sockfd);
    }


    return 0;
}
