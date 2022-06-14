#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#include "tools/tools.h"
#include "http/http.h"

#define BUF_SIZE 1024
//#define MAXLINE 128

//void myError(int, const char *);

int main(int argc, char *argv[]){
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if(argc != 2){      //后面用config文件代替
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }

    listenfd = openListen(argv[1]);
    while(1){
        connfd = accept(listenfd, (sockaddr *)&clientaddr, &clientlen);
        getnameinfo((const sockaddr *)&clientaddr, clientlen, hostname, MAXLINE, port, MAXLINE, 0);
        fprintf(stdout, "Accept connection from (%s : %s)\n", hostname, port);

        handleRequest(connfd);
        close(connfd);
    }
    return 0;
}
