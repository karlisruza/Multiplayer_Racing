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
#include "./connect/listen.h"
#include "../data/message.h"
#include "../data/protocol.h"
#include "../data/game/listtest.h"
#include "./clientthread/clientthread.h"

int main(void){
    int serverFd = startListen();

    playerlist_t* playerlist = malloc(sizeof(playerlist_t));
    gamelist_t* gamelist = malloc(sizeof(gamelist_t));
    getTestingList(&gamelist);
    printGameList(&gamelist);

    while(true){
        	struct sockaddr_in peerAddr;
	        socklen_t addrSize = sizeof(peerAddr);
	        int clientFd = accept(serverFd, (struct sockaddr *) &peerAddr, &addrSize);
            if (clientFd == -1) {
                perror("accept");
                return -1;
            }
            printClient(clientFd);	
            player_t* player = malloc(sizeof(player_t));
            player->ID = clientFd;
            playerlistPush(&playerlist, &player);
            printPlayerList(&playerlist);

            //so that the list is passed and thread knows its fd
            params_t* params = malloc(sizeof(struct threadParam));
            params->clientFd = clientFd;
            params->serverFd = serverFd;
            params->gameList = gamelist;
            params->playerList = playerlist;
            
            pthread_t thread;
	        pthread_create(&thread, NULL, clientThread, (void *) params);
    }
}