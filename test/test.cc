#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    char *host, **name, **addrs;

    struct hostnet *hostinfo;
    if(argc == 1){
        char myname[256];
        gethostname(myname, 255);
        host = myname;
    }
    else{
        host = argv[1];
    }
    printf("computer name = %s\n", host);
    hostinfo = gethostbyname(host);
    if(!hostinfo){
        fprintf(stderr, "cannot get info for host: %s\n", host);
        exit(1);
    }
    return 0;
}