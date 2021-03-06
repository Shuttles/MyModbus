/*************************************************************************
	> File Name: head.h
	> Author: 少年宇
	> Mail: 
	> Created Time: 2021年06月14日 星期一 10时57分37秒
 ************************************************************************/

#ifndef _HEAD_H
#define _HEAD_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdbool.h>

#define MAX_N 3
#define PASSWORD 9973
#define IS_READ(a) ((a >= 1) && (a <= 4))
#define IS_WRITE(a) ((a >= 5) && (a <= 8))

#endif
