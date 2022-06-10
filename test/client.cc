#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>



int main(int argc, char *argv[]){
    int sockfd; // 
    int len;
    struct sockaddr_in addr;
    int ret;
    char ch = 'A';
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("192.168.31.4");
    addr.sin_port = htons(8081);

    len = sizeof(addr);

    ret = connect(sockfd, (sockaddr *)&addr, len);
    if(ret == -1){
        perror("oops:client");
        exit(1);
    }
    write(sockfd, &ch, 1);
    read(sockfd, &ch, 1);
    printf("char from server = %c\n", ch);
    close(sockfd);
    return 0;
}