#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <netdb.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <errno.h>
#include "crypto.h"
#include "mbedtls/aes.h"

#define MAX_SIZE    1024
#define PORT        9875

int client_setup(int *sockfd, struct sockaddr_in *servaddr);
void teardown(int sockfd, FILE *fp);


int main(int argc, char *argv[])
{
    create_aes_key();
    // define a buffer of max size
    unsigned char buf[MAX_SIZE] = { 0 };
    void *sent;
    int bytes_read = 0;
    int bytes_written = 0;
    FILE *fp;
    int fd;
    int sockfd = 0;
    struct sockaddr_in servaddr = { 0 };
    // open a socket to the destination server
    if(client_setup(&sockfd, &servaddr) < 0) {
        exit(0);
    }
    // while there is still content in the file
    if((fp = fopen(argv[1], "r")) == NULL) {
        printf("Failed to read file, dead\n");
        exit(0);
    }
    fd = fileno(fp);

    // send the filename over and get the confirmation
    // the confirmation is just the filename
    unsigned char fname[] = "test_file.txt";
    bytes_written = write(sockfd, fname, sizeof(fname)-1);
    bytes_read = read(sockfd, buf, sizeof(buf));

    if(strncmp(buf, fname, strlen(fname)+1) != 0) {
        printf("__%s__ __%s__\n", buf, fname);
        printf("strings aren't equal\n");
        teardown(sockfd, fp);
        return 0;
    }
    bytes_written = 0;
    bytes_read = 0;

    // send the whole file
    while(1) {
        bytes_read = read(fd, buf, sizeof(buf));
        if(bytes_read == 0) {
            break; // we're done reading
        } else if(bytes_read < 0) {
            printf("error reading file: %s\n", strerror(errno));
            teardown(sockfd, fp);
            exit(0);
        }
    
        // send the content through the socket
        sent = buf;
        while(bytes_read > 0) {
            bytes_written = write(sockfd, sent, bytes_read);
            if(bytes_written < 0) {
                printf("error writing: %s\n", strerror(errno));
                teardown(sockfd, fp);
                break;
            }
            bytes_read -= bytes_written;
            sent += bytes_written;
        }
    }
    return 0;
}

// build the socket and connect to the server
int client_setup(int *sockfd, struct sockaddr_in *servaddr)
{ 
    struct hostent *he;
    struct sockaddr_in dest_addr;

    // make sure the destination address if valid
    if ((he=gethostbyname("127.0.0.1")) == NULL) {
        herror("gethostbyname");
        return -1;
    }

    // create the socket to connect to the destination server
     if ((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    dest_addr.sin_family = AF_INET;      // host byte order
    dest_addr.sin_port = htons(PORT);    // short, network byte order 
    dest_addr.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(dest_addr.sin_zero), 8);     // zero the rest of the struct

    // connect to the server
    if (connect(*sockfd, (struct sockaddr *)&dest_addr, \
                                            sizeof(struct sockaddr)) == -1) {
        perror("connect");
        return -1;
    }
    return 0;
}

void teardown(int sockfd, FILE *fp) 
{
    printf("teardown\n");
    close(sockfd);
    fclose(fp);
}
