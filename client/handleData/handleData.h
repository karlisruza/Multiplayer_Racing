#include "handleMessage/handlePlayerJoined.h"
#include "handleMessage/handlePlayerLeft.h"
#include "handleMessage/handleStartGame.h"
#include "handleMessage/handleUpdatePlayer.h"
#include "../../data/message.h"

#ifndef HANDLEDATA_H_INCLUDED
#define HANDLEDATA_H_INCLUDED

void handleData(msg_t* message, playerlist_t** list, player_t* clientPlayer, int clientFd){
    playerlist_t* playerList = *list; 
    switch (message->type){
        case PLAYER_JOINED:
            handlePlayerJoined(message, &playerList, clientFd);
            break;
        case PLAYER_LEFT:
            handlePlayerLeft(message, &playerList, clientFd);
            break;
        case START_GAME:
            handleStartGame(&playerList, clientPlayer, clientFd);
            break;
        case UPDATE_PLAYER:
            handleUpdatePlayer(message, &playerList);
            break;
        default:
            perror("invalid message");
    }
}

#endif