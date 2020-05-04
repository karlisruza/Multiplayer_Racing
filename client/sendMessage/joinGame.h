#include "./requestPlayer.h"
#ifndef JOINGAME_H_INCLUDED
#define JOINGAME_H_INCLUDED

void joinGame(playerlist_t** list, player_t** player, int clientFd){
    msg_t request;
    request.type = JOIN_GAME;
    playerlist_t* playerList = *list;
    player_t* clientPlayer = *player;

    cg_pt payload;
    payload.gameID = clientPlayer->gameID;
    payload.playerID = clientPlayer->ID;
    
    memcpy((void*)&request.payload, (void*)&payload, sizeof(payload));
    int length = ((void*)&request.payload - (void*)&request.type) + sizeof(payload);
    sendData(clientFd, (void*)&request, length, NULL);

    void* buffer = (void*)malloc(sizeof(msg_t));
    length = sizeof(msg_t);
    int retLen = recv(clientFd, (void*)buffer, length, 0);
    if(retLen < 0){
        printf("fail \n");
    }
    
    return;
}

#endif