
#include "../../calculatePosition/position.h"

#ifndef HANDLESUPDATEPLAYER_H_INCLUDED
#define HANDLESUPDATEPLAYER_H_INCLUDED



void handleUpdatePlayer(msg_t* message, gamelist_t** list, int clientFd){
    gamelist_t* gameList = *list;
    up_pt* payload = (up_pt*)message->payload;
    action_t action = payload->action;
    playerlist_t* playerList;;
    game_t* current = gameList->head;

    printf("updateplayer playerid: %d\n", payload->playerID);
    printf("gameid: %d\n", payload->gameID);

    //find playerList for game in question
    while(current != NULL){
        if(current->gameid == payload->gameID){
            playerList = current->playerList;
            break;
        }
        current = current->next;
    }
    player_t* currentPlayer = playerList->head;
    //find player to update
    while(currentPlayer != NULL){
        if(currentPlayer->ID == clientFd){
            break;
        }
        currentPlayer = currentPlayer->next;
    }

    //updates player position
    updatePlayer(&currentPlayer, action);
    //prepare message to send to all game members
    msg_t reply;
    reply.type = UPDATE_PLAYER;
    updpos_pt updatedPlayer;
    updatedPlayer.ID = currentPlayer->ID;
    updatedPlayer.x = currentPlayer->x;
    updatedPlayer.y = currentPlayer->y;
    updatedPlayer.angle = currentPlayer->angle;
    printf("from hup: %f\n", updatedPlayer.x);

    memcpy((void*)&reply.payload, (void*)&updatedPlayer, sizeof(updatedPlayer));
    int length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(updatedPlayer);

    //send to all game members
    currentPlayer = playerList->head;
    while(currentPlayer != NULL){
        sendData(currentPlayer->ID, (void*)&reply, length, NULL);
        currentPlayer = currentPlayer->next;
    }

    printf("update player done\n");
    return;
}

#endif