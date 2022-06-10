#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include <errno.h>

#define BUF_SIZE 1024

void myError(int, const char *);

int main(int argc, char *argv[]){
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    int ret;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    char BUF[BUF_SIZE];
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    //INADDR_ANY
    //server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8081);
    server_len = sizeof(server_addr);

    ret = bind(server_sockfd, (sockaddr *)&server_addr, server_len);
    myError(ret, "bind");
    ret = listen(server_sockfd, 5);
    myError(ret, "listen");

    char response[] = "HTTP/1.1 200 OK\n\n<html><body><h1>Hello</h1></body></html>";
    while(1){
        printf("server waiting\n");
        client_len = sizeof(client_addr);
        client_sockfd = accept(server_sockfd, (sockaddr *)&client_addr, (socklen_t *)&client_len);
        printf("hello\n");
        //write(cl)
        myError(client_sockfd, "accept");
        
        char ch;
        read(client_sockfd, BUF, BUF_SIZE);
        printf("data : %s\n", BUF);
        
        write(client_sockfd, response, strlen(response));
        //read(client_sockfd, &ch, 1);
        //++ch;
        //write(client_sockfd, &ch, 1);
        close(client_sockfd);
    }

    return 0;
}

void myError(int ret, const char *errinfo){
    if(ret == -1){
        printf("%s error\n", errinfo);
        if(errno == EDOM){
            printf("hello\n");
            perror("error");
            printf("hello\n");
        }
    }
}