#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h> 
#include "../car/car.h"
#include "../protocol/message.h"


int main(int argc, char* argv[]){
    char* ptr;
    int cPort = strtol(argv[1], &ptr, 10);
    int sPort = strtol(argv[3], &ptr, 10);
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
    inet_aton(argv[2], &serverAddr.sin_addr);
    //connect to server address
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));

   

    msg_t message;
    message.type = PING;

    int length = ((void*)&message.payload - (void*)&message.type);
    msg_t* msgr;
    int i = 0;
    while(i < 10){
        void* buffer = malloc(sizeof(MAX_PAYLOAD_SIZE+sizeof(msg_t)));
        send(clientSocket, (void*)&message, length, 0);
        recv(clientSocket, buffer, sizeof(msg_t), 0);
        msgr = (msg_t*)buffer;
        printf("b4 handle %d \n", msgr->type);
        handleData(msgr, clientSocket);

        free(buffer);
        i++;
    }
}