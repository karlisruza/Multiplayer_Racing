#include "./game/game.h"

#ifndef THREADPARAM_H_INCLUDED
#define THREADPARAM_H_INCLUDED

typedef struct threadParam{
    int clientFd;
    int serverFd;
    playerlist_t* playerList;
    gamelist_t* gameList;
    pthread_mutex_t* lock;
}params_t;

#endif

