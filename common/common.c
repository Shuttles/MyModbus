/*************************************************************************
	> File Name: common.c
	> Author: 少年宇
	> Mail: 
	> Created Time: 2021年06月14日 星期一 10时59分18秒
 ************************************************************************/

#include "head.h"

char *get_value(char *path, char *key) {
    FILE *fp = NULL;
    ssize_t nread;
    char *line = NULL, *sub = NULL;
    extern char conf_ans[50];
    size_t linecap;
    if (path == NULL || key == NULL) {
        fprintf(stderr, "Error in argument!\n");
        return NULL;
    }
    if ((fp = fopen(path, "r")) == NULL) {
        perror("fopen");
        return NULL;
    }

    while ((nread = getline(&line, &linecap, fp)) != -1) {
        if ((sub = strstr(line, key)) == NULL) continue;
        else {
            if (line[strlen(key)] == '=') {
                strncpy(conf_ans, sub + strlen(key) + 1, nread - strlen(key) - 2);
                *(conf_ans + nread - strlen(key) - 2) = '\0';
                break;
            }
        }
    }
    free(line);
    fclose(fp);
    return (sub == NULL ? NULL : conf_ans);
}

