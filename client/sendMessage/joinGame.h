#include "./requestPlayer.h"
#ifndef JOINGAME_H_INCLUDED
#define JOINGAME_H_INCLUDED

int joinGame(playerlist_t** list, player_t** player, int clientFd, int gameId){
    msg_t request;
    request.type = JOIN_GAME;
    playerlist_t* playerList = *list;
    player_t* clientPlayer = *player;

    cg_pt payload;
    payload.gameID = gameId;
    payload.playerID = clientPlayer->ID;

    memcpy((void*)&request.payload, (void*)&payload, sizeof(payload));
    int length = ((void*)&request.payload - (void*)&request.type) + sizeof(payload);
    sendData(clientFd, (void*)&request, length, NULL);

    requestPlayer(&playerList, clientFd);

    return 1;
}

#endif