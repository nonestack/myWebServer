#ifndef __HTTP_H__
#define __HTTP_H__

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "../tools/tools.h"

#define MAXLINE 1024

void handleRequest(int fd);
void handleRequestHeader(rio_t *rp);
int parseURI(char *uri, char *filename, char *cgiargs);         //return 1 is static content, return 0 is dynamic content

void serveStatic(int fd, char *filename,  size_t filesize);
void serveDynamic(int fd, char *filename, char *cgiargs);

void getFileType(char *filename, char *filetype);

#endif