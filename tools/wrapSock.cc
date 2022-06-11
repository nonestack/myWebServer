#include "tools.h"

int openListen(char *port){
    struct addrinfo hints, *listp, *p;
    int listenfd, optval = 1;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;

    getaddrinfo(NULL, port, &hints, &listp);

    for(p = listp; p != NULL; p = p->ai_next){
        int ret;
        printf("world\n");
        listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(listenfd == -1)
            continue;

        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(optval));
        ret = bind(listenfd, p->ai_addr, p->ai_addrlen);
        if(ret == 0)
            break;
        close(listenfd);
    }

    freeaddrinfo(listp);
    if(p == NULL){
        return -1;
    }

    if(listen(listenfd, 1024) == -1){
        close(listenfd);
        return -1;
    }

    return listenfd;
}