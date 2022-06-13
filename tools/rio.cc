#include "tools.h"

//无缓冲
ssize_t rio_readn(int fd, void *usrbuf, size_t n){
    size_t nleft = n;
    ssize_t nread;
    char *bufp = (char *)usrbuf;

    while(nleft > 0){
        if((nread = read(fd, bufp, nleft)) < 0){    //执行出错
            if(errno == EINTR)                      //信号打断
                nread = 0;
            else
                return -1;
        }
        else if(nread == 0)
            break;
        nleft -= nread;
        bufp += nread;
    }
    return n - nleft;
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n){
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = (char *)usrbuf;

    while(nleft > 0){
        if((nwritten = write(fd, bufp, nleft)) < 0){
            if(errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}


//有缓冲
void rio_readinitb(rio_t *rp, int fd){
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_read(rio_t *rp, char *buf, size_t n){
    int cnt;

    while(rp->rio_cnt <= 0){            //如果buf为空，重新填充
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if(rp->rio_cnt < 0){
            if(errno != EINTR)
                return -1;
        }
        else if(rp->rio_cnt == 0){      // EOF
            return 0;
        }
        else{
            rp->rio_bufptr = rp->rio_buf;
        }
    }

    cnt = n;
    if(rp->rio_cnt < n)
        cnt = rp->rio_cnt;
    memcpy(buf, rp->rio_bufptr, cnt);
    rp->rio_cnt -= cnt;
    rp->rio_bufptr += cnt;
    return cnt;
}

ssize_t rio_readlineb(rio_t *rp, void *buf, size_t maxlen){
    int n, rc;
    char c, *bufp = (char *)buf;
    for(n = 1; n < maxlen; ++n){
        if((rc = read(rp->rio_fd, &c, 1) == 1)){
            *bufp = c;
            ++bufp;
            if(c == '\n'){
                ++n;
                break;
            }
        }
        else if(rc == 0){
            if(n == 1){             //开始就读到EOF
                return 0;
            }
            else{
                break;
            }
        }
        else{                       //-1, error
            return -1;
        }
    }
    *bufp = 0;
    return n - 1;
}

ssize_t rio_readnb(rio_t *rp, void *buf, size_t n){
    size_t nleft = n;
    ssize_t nread;
    char *bufp = (char *)buf;

    while(nleft > 0){
        if((nread = rio_read(rp, (char *)buf, nleft)) < 0){
            return -1;              //rio_read return -1, 信号打断
        }
        else if(nread == 0){
            break;                  //EOF
        }
        nleft -= nread;
        bufp += nread;
    }
    return n - nleft;
}