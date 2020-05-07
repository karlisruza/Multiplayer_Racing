
#include "../../sendMessage/requestUpdatePlayer.h"

#ifndef HANDLESTARTGAME_H_INCLUDED
#define HANDLESTARTGAME_H_INCLUDED

int handleStartGame(playerlist_t** list, player_t* clientPlayer, int clientFd){
    // playerlist_t* playerList = *list;
    // requestUpdatePlayer(&playerList, clientPlayer, clientFd, 0, 0);
    printf("GAMESTART!!!\n");
    return 0;
}

#endif