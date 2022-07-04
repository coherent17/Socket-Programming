#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 255
#define BACKLOG 5

void error(const char *msg){
    //print the error message to stderr
    perror(msg);
    exit(1);
}

//argc = 2, filenamd & port number
int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "Port No not provided. Program terminated\n");
        exit(1);
    }

    int sockfd, newsockfd, portno, n;
    char buffer[BUFFER_SIZE];

    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        error("Error opening Socket.");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        error("Binding Failed");
    }

    listen(sockfd, BACKLOG);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if(newsockfd < 0){
        error("Error on Accepting");
    }

    //communication between client and server
    while(1){
        bzero(buffer, BUFFER_SIZE);
        n = read(newsockfd, buffer, BUFFER_SIZE);
        if(n < 0){
            error("Error on reading");
        }
        //print out the string that client transfer to server
        printf("Client: %s\n", buffer);
        bzero(buffer, BUFFER_SIZE);

        //read whole line that server want to transfer to client into buffer string
        fgets(buffer, BUFFER_SIZE, stdin);
        n = write(newsockfd, buffer, BUFFER_SIZE);
        if(n < 0){
            error("Error on Writing");
        }

        //if server reply "Bye", break the infinite loop
        if(!strncmp("Bye", buffer, strlen("BYE"))){
            break;
        }
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}