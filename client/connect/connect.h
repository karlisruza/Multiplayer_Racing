#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef CONNECT_H_INCLUDED
#define CONNECT_H_INCLUDED

int clientConnect(char* arg1, char* arg2, char* arg3){
    //Connection setup
    char* ptr;
    int cPort = strtol(arg1, &ptr, 10);
    int sPort = strtol(arg3, &ptr, 10);
    if(!cPort || !sPort){
        perror("invalid args");;
    }

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("tcp socket error");
        return -1;
    }
    //set protocol to ipv4 and set port
    struct sockaddr_in myAddr;
    memset(&myAddr, 0, sizeof(struct sockaddr_in));
    myAddr.sin_family = AF_INET; //set family to ipv4
    myAddr.sin_port = htons(cPort); //htons converts to network byte order

    //bind address to socket
    if (bind(clientSocket, (struct sockaddr *) &myAddr, sizeof(struct sockaddr_in)) == -1) {
        perror("tcp bind");
        return -1;
    }

    //server address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(sPort);
    inet_aton(arg2, &serverAddr.sin_addr);
    //connect to server address
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));
    return clientSocket;
}

#endif