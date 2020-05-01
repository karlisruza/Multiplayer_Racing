#include <stdio.h>
#include <stdlib.h>
#include "../../data/protocol.h"
#include "../../data/game/game.h"
#include "../../data/payload/payload.h"
#include "../../data/message.h"
#include "../../data/game/listtest.h"

#ifndef HANDLEDATA_H_INCLUDED
#define HANDLEDATA_H_INCLUDED

void handleData(msg_t *message, int clientFd, gamelist_t** gamelist, player_t** clientplayer){
    msg_t reply;
    up_pt* data;
    cg_pt* gameinfo;
    int length;

    printf("handleData enum %d\n", message->type);
    switch (message->type){
        case 1:
            printf("lmao\n");
            data = (up_pt*)(message->payload);
            printf("playerId %d", data->playerID);
            printf("gameId %d", data->playerID);
            printf("keypress x: %d", data->action.x);
            printf("keypress y: %d", data->action.y);
            printf("lmao\n");
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7: //ping
            reply.type = PONG;
            length = ((void*)&reply.payload - (void*)&reply.type);
            sendData(clientFd, (void*)&reply, length, NULL);
            break;
        case 8:
            printf("PONG\n");
            break;
        case REQUEST_GAME:{
            
            break;
        }
        case 10:{
            gameinfo = (cg_pt*)message->payload;
            printf("gameId: %d\n", gameinfo->gameID);
            printf("gameId: %d\n", gameinfo->playerID);
            printf("case 11\n");
            break;
        }
        case ADD_PLAYER:{
            break;
        }
        default:
            perror("invalid message");
    }
}

#endif