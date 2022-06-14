#include "http.h"

void handleRequest(int fd){
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    int is_static = 0;
    struct stat sbuf;
    rio_t rio;

    rio_readinitb(&rio, fd);
    rio_readlineb(&rio, buf, MAXLINE);

    printf("Request headers:\n");
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);
    //printf("method : %s\n", method);
    //printf("uri : %s\n", uri);
    //printf("version : %s\n", version);
    handleRequestHeader(&rio);
    is_static = parseURI(uri, filename, cgiargs);
    //printf("filename = %s\n", filename);
    //printf("cgi-args = %s\n", cgiargs);

    if(stat(filename, &sbuf) < 0){
        //处理错误 返回404...
        return ;
    }
    if(is_static == 1){         //static content
        if(!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)){      //relative information in man inode
            //处理错误 403
        }
        serveStatic(fd, filename, sbuf.st_size);

    }
    else{                       //dynamic content
        if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)){      //relative information in man inode
            //处理错误 403
        }
        serveDynamic(fd, filename, cgiargs);
    }
    return;
}

void handleRequestHeader(rio_t *rp){
    char buf[MAXLINE];

    rio_readlineb(rp, buf, MAXLINE);
    while(strcmp(buf, "\r\n") != 0){
        rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    return;
}

int parseURI(char *uri, char *filename, char *cgiargs){
    char *ptr;

    if(strstr(uri, "cgi-bin") == NULL){         //static content
        strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        if(uri[strlen(uri) - 1] == '/'){
            strcat(filename, "index.html");
        }
        return 1;
    }
    else{
        ptr = index(uri, '?');
        if(ptr != NULL){
            strcpy(cgiargs, ptr+1);
            *ptr = '\0';
        }
        else{
            strcpy(cgiargs, "");
        }
        strcpy(filename, ".");
        strcat(filename, uri);
        return 0;
    }
}

void serveStatic(int fd, char *filename, size_t filesize){
    int srcfd;
    char *srcp, filetype[MAXLINE], buf[RIO_BUFSIZE];

    getFileType(filename, filetype);
    //send http header to web brower
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: My Web Server\r\n", buf);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sConnect-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sConnect-type: %s\r\n", buf, filetype);
    rio_writen(fd, buf, strlen(buf));
    printf("Response header:\n%s\n", buf);

    //send http body to web brower
    srcfd = open(filename, O_RDONLY, 0);
    srcp = (char *)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    close(srcfd);
    rio_writen(fd, srcp, filesize);
    munmap(srcp, filesize);
}

void serverDynamic(int fd, char *filename, char *cgiargs){

}


void getFileType(char *filename, char *filetype){
    if(strstr(filename, "html")  != NULL){
        strcpy(filetype, "text/html");
    }
    else if(strstr(filename, "gif")  != NULL){
        strcpy(filetype, "image/gif");
    }
    else if(strstr(filename, "png")  != NULL){
        strcpy(filetype, "image/png");
    }
    if(strstr(filename, "jpg")  != NULL){
        strcpy(filetype, "image/jpg");
    }
    else{
        strcpy(filetype, "text/plain");
    }
}