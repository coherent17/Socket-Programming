#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 255

void error(const char *msg){
    //print the error message to stderr
    perror(msg);
    exit(1);
}

//argc = 3, filename, server_ipaddress, portno
int main(int argc, char *argv[]){
    if(argc < 3){
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[BUFFER_SIZE];
    
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error opening socket");
    }

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr, "Error, no such host");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    //connect to server
    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        error("Connection Failed");
    }

    while(1){
        bzero(buffer, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        n = write(sockfd, buffer, BUFFER_SIZE);
        if(n < 0){
            error("Error On Writing");
        }
        bzero(buffer, BUFFER_SIZE);
        n = read(sockfd, buffer, BUFFER_SIZE);
        if(n < 0){
            error("Error On Reading");
        }

        //print the string from server
        printf("Server: %s\n", buffer);
        if(!strncmp("Bye", buffer, strlen("BYE"))){
            break;
        }
    }
    close(sockfd);
    return 0;
}