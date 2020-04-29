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
#include "../data/message.h"
#include "../data/protocol.h"
#include "../data/racinglist.h"
#include "../data/listtest.h"

#define PORT 8005
#define _MAX_LISTEN_QUE 10

void *clientThread(void* param){
    gamelist_t* gamelist = ((params_t*)param)->list;

        void* buffer = malloc(sizeof(MAX_PAYLOAD_SIZE+sizeof(msg_t)));

        int length = (sizeof(msg_t)) + MAX_PAYLOAD_SIZE;

        int retLen = recv(((params_t*)param)->clientFd, buffer, length, 0);
        if(retLen < 0){
            printf("fail \n");
        }
        msg_t* msgr = (msg_t*)buffer;
        handleDataS(msgr, (params_t*)param);

        retLen = recv(((params_t*)param)->clientFd, buffer, length, 0);
        if(retLen < 0){
            printf("fail \n");
        }
        
        handleDataS(msgr, (params_t*)param);

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

    playerlist_t* playerlist = malloc(sizeof(playerlist_t));
    gamelist_t* gamelist = malloc(sizeof(gamelist_t));
    getTestingList(&gamelist);

    while(true){
        	struct sockaddr_in peerAddr;
	        socklen_t addrSize = sizeof(peerAddr);
	        int clientFd = accept(serverSocket, (struct sockaddr *) &peerAddr, &addrSize);
            if (clientFd == -1) {
                perror("accept");
                return -1;
            }
            printClient(clientFd);	
            player_t player;
            player.ID = clientFd;

            //so that the list is passed and thread knows its fd
            params_t* params = malloc(sizeof(struct threadParam));
            params->clientFd = clientFd;
            params->serverFd = serverSocket;
            params->list = gamelist;
            params->clientlist = playerlist;
            
            // pushPlayer(&players, clientFd);

            pthread_t thread;
	        pthread_create(&thread, NULL, clientThread, (void *) params);
    }
}