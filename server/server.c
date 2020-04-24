#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../car/car.h"
#include "../car/player.h"
#include "../protocol/message.h"

#define PORT 8081
#define _MAX_LISTEN_QUE 10

struct threadParam{
    int clientFd;
    int serverFd;
    struct playerList* list;
};

//this functions takes buffer of type char and converts string int/int/int/int/int/int/int/ to seperate ints
//and places data into struct car passed
void parseBuffer(char** buffer, struct car** player){
        int slashCount = 0;
        int counter = 0;
        int intLength = 0;

        char* temp = malloc(sizeof(char)*256);
        // printf("buffer in parse fn \n %s \n", *buffer);
        while(slashCount < 9){
            if((*buffer)[counter] > 47 && (*buffer)[counter] < 58){
                temp[intLength] = (*buffer)[counter];
                temp[intLength + 1]= '\0';
                intLength++;
            }
            else if((*buffer)[counter] == '/'){
                switch (slashCount){
                    case 0:
                        (*player)->playerId = atoi(temp);
                        break;
                    case 1:
                        (*player)->head->x = atoi(temp);
                        break;
                    case 2:
                        (*player)->head->y = atoi(temp);
                        break;
                    case 3:
                        (*player)->mid->x = atoi(temp);
                        break;
                    case 4:
                        (*player)->mid->y = atoi(temp);
                        break;
                    case 5:
                        (*player)->tail->x = atoi(temp);
                        break;
                    case 6:
                        (*player)->tail->y = atoi(temp);
                        break;
                    case 7:
                        printf("case 7 \n");
                        (*player)->angle = atoi(temp);
                        break;
                    default:
                        perror("Corrupt data while parsing buffer");                                                 
                }
                printf("%d", slashCount);
                slashCount++;
                intLength = 0;
            }
            else{
                return;
            }
            // printf("%s \n", temp);
            counter++;
        }
}

void *clientThread(void* param){
    msg_t message;
    message.type = PONG;
    message.payload[0] = 'P';
    message.payload[1] = 'O';
    message.payload[2] = 'N';
    message.payload[3] = 'G';


    while(true){
        void* buffer = malloc(sizeof(MAX_PAYLOAD_SIZE+sizeof(msg_t)));

        int length = (sizeof(msg_t)) + MAX_PAYLOAD_SIZE;

        int retLen = recv(((struct threadParam*)param)->clientFd, buffer, length, 0);
        if(retLen < 0){
            printf("fail \n");
        }
        msg_t* msgr = (msg_t*)buffer;
        handleData(msgr, ((struct threadParam*)param)->clientFd);

        printf("Enum %d\n", msgr->type);
        printf("Enum %s\n", msgr->payload);

        length = ((void*)&message.payload - (void*)&message.type) + 4;

        sendData(((struct threadParam*)param)->clientFd, (void*)&message, length, NULL);
        free(buffer);
    }


    // while(true){        
    //     if(retLen > 0){
    //             parseBuffer(&buffer, &player);
    //             printCar(player);
    //     }
    //     printf("thread alive\n");
    //     send(((struct threadParam*)param)->clientFd, data, sizeof(char)*256, 0);
    // }  
}

int main(void){
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("tcp socket error");
        return -1;
    }
    
    //set protocol to ipv4 and set port
    struct sockaddr_in myAddr;
    memset(&myAddr, 0, sizeof(struct sockaddr_in));
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(PORT); //htons converts to network byte order

    //bind address to socket
    if (bind(serverSocket, (struct sockaddr *) &myAddr, sizeof(struct sockaddr_in)) == -1) {
        perror("tcp bind");
        return -1;
    }

    //set socket to listen
    if (listen(serverSocket, _MAX_LISTEN_QUE) == -1) {
        perror("listen");
        return -1;
    }
    printf("Listening on port %d \n", PORT);

    struct playerList* players = malloc(sizeof(struct playerList));
    while(true){
        	struct sockaddr_in peerAddr;
	        socklen_t addrSize = sizeof(peerAddr);
	        int clientFd = accept(serverSocket, (struct sockaddr *) &peerAddr, &addrSize);
            if (clientFd == -1) {
                perror("accept");
                return -1;
            }
            printClient(clientFd);	

            //so that the list is passed and thread knows its fd
            struct threadParam* params = malloc(sizeof(struct threadParam));
            params->clientFd = clientFd;
            params->serverFd = serverSocket;
            params->list = players;

            pushPlayer(&players, clientFd);

            pthread_t thread;
	        pthread_create(&thread, NULL, clientThread, (void *) params);
    }
}