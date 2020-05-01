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

    playerlist_t* playerList = malloc(sizeof(playerlist_t));
    gamelist_t* gameList = malloc(sizeof(gamelist_t));
    gameList->count = 0;
    getTestingList(&gameList);
    printGameList(&gameList);

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
            playerlistPush(&playerList, &player);
            printPlayerList(&playerList);

            //packs params for use in thread
            params_t* params = malloc(sizeof(struct threadParam));
            params->clientFd = clientFd;
            params->serverFd = serverFd;
            params->gameList = gameList;
            params->playerList = playerList;
            
            pthread_t thread;
	        pthread_create(&thread, NULL, clientThread, (void *) params);
    }
}