#include "http.h"

void handleRequest(int fd){
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    rio_t rio;

    rio_readinitb(&rio, fd);
    rio_readlineb(&rio, buf, MAXLINE);

    printf("Request headers:\n");
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);
    //printf("method : %s\n", method);
    //printf("uri : %s\n", uri);
    //printf("version : %s\n", version);
}