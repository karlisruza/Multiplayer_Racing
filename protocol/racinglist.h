#include <stdio.h>
#include <stdlib.h>
#include "./protocol.h"

#ifndef RACINGLIST_H_INCLUDED
#define RACINGLIST_H_INCLUDED
#define MAX_PLAYERS_COUNT 8
#define P_START_X 111
#define P_START_Y 222
#define P_START_ANGLE 0

void playerlistPush(playerlist_t *list){
    player_t player;
    player.angle = P_START_ANGLE;
    player.position.x = P_START_X;
    player.speed = 0;
    player.acceleration = 0;
    player.laps = 0;
    
    if(list->head != NULL){
        player.ID = 1;
        player.position.y = P_START_Y;
        list->head = &player;
        list->tail = &player;
        list->count++;
    }
    else{
        player.ID = list->tail->ID + 1;
        player.position.y = P_START_Y + list->count*2;
        player.prev = list->tail;
        list->tail->next = &player;
        list->count++;
    }
    //:TODO SEND PLAYER ID TO PLAYER
    return;
}

void gamelistPush(gamelist_t *list){
    game_t game;
    playerlistPush(game.playerlist);
    if(list->head != NULL){
        game.gameid = 1;
        game.prev = list->tail;
        list->head = &game;
        list->tail = &game;
    }
    else{
        game.gameid = list->tail->gameid + 1;
        list->tail->next = &game;
        list->count++;
    }
    //TODO SEND GAME ID TO PLAYER
}

// void pushPlayer(struct playerList** list, int clientFd){
//     struct player* client = malloc(sizeof(struct player));
//     client->clientSocket = clientFd;
//     client->carData = malloc(sizeof(struct car));
//     initCar(&client->carData);
//     if((*list)->head == NULL){
//         (*list)->head = client;
//         (*list)->tail = client;
//         (*list)->playerCount++;
//     }
//     else{
//         (*list)->tail->next = client;
//         client->prev = (*list)->tail;
//         (*list)->tail = client;
//         (*list)->playerCount++;
//     }
//     return;
// }


#endif