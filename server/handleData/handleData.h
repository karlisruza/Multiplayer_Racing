#include "./handleType/ping.h"
#include "./handleType/handleAddPlayer.h"
#include "./handleType/handleRequestGame.h"
#include "./handleType/handleCreateGame.h"
#include "./handleType/handleJoinGame.h"
#include "./handleType/handleRequestPlayer.h"
#include "./handleType/handleStartGame.h"
#include "./handleType/handleUpdatePlayer.h"

#ifndef HANDLEDATA_H_INCLUDED
#define HANDLEDATA_H_INCLUDED

void handleData(msg_t *message, params_t* params){
    msg_t reply;
    up_pt* data;
    gamelist_t* gameList = params->gameList;;
    playerlist_t* playerList = params->playerList;
    int clientFd = params->clientFd;
    int length;

    printf("handleData enum %d\n", message->type);
    switch (message->type){

        case PING: //ping
            ping(clientFd);
            break;
        case PONG:
            printf("PONG\n");
            break;
        case REQUEST_GAME:
            handleRequestGame(&gameList, clientFd);
            break;
        case CREATE_GAME:
            handleCreateGame(&gameList, &playerList, clientFd);
            break;
        case ADD_PLAYER:{
            handleAddPlayer(message, clientFd, &playerList);
            break;
        }
        case REQUEST_PLAYER:
            handleRequestPlayer(message, &gameList, clientFd);
            break;
        case JOIN_GAME:{
            handleJoinGame(message, &gameList, &playerList, clientFd);  
            break;
        }
        case START_GAME:{
            handleStartGame(message, &gameList, clientFd);
            break;
        }
        case UPDATE_PLAYER:{
            handleUpdatePlayer(message, &gameList, clientFd);
            break;
        }
        default:
            perror("invalid message");
    }
}

#endif