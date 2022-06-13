#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//wrapSock.cc
int openListen(char *port);


//rio.cc
//带缓冲io
ssize_t rio_readn(int fd, void *buf, size_t bufn);
ssize_t rio_writen(int fd, void *buf, size_t bufn);

//不带缓冲io
#define RIO_BUFSIZE 8192

typedef struct {
    int rio_fd;                     
    int rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFSIZE]; 
} rio_t;

void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readlineb(rio_t *rp, void *buf, size_t maxlen);
ssize_t rio_readnb(rio_t *rp, void *buf, size_t n);

#endif
