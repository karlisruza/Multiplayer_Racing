#include <stdio.h>
#include <stdlib.h>
#include "./protocol.h"

#ifndef RACINGLIST_H_INCLUDED
#define RACINGLIST_H_INCLUDED
#define MAX_PLAYERS_COUNT 8
#define P_START_X 111
#define P_START_Y 222
#define P_START_ANGLE 0

void playerlistPush(playerlist_t** list){
    player_t* player = malloc(sizeof(player_t));
    player->angle = P_START_ANGLE;
    player->position.x = P_START_X;
    player->speed = 0;
    player->acceleration = 0;
    player->laps = 0;
    player->next = NULL;
    player->prev = NULL;
    
    if((*list)->head == NULL){
        player->ID = 1;
        player->position.y = P_START_Y;
        (*list)->head = player;
        (*list)->tail = player;
        (*list)->count++;
        return;
    }
    else{
        player->ID = (*list)->tail->ID + 1;
        player->position.y = P_START_Y + (*list)->count*2;
        (*list)->tail->next = player;
        player->prev = (*list)->tail;
        (*list)->tail = player;
        (*list)->count++;
    }
    //:TODO SEND PLAYER ID TO PLAYER
    return;
}

void gamelistPush(gamelist_t** list){
    game_t* game = malloc(sizeof(game_t));
    game->playerlist = malloc(sizeof(playerlist_t));
    playerlistPush(&game->playerlist);
    if((*list)->head == NULL){
        game->gameid = 1;
        game->prev = (*list)->tail;
        (*list)->head = game;
        (*list)->tail = game;
    }
    else{
        game->gameid = (*list)->tail->gameid + 1;
        (*list)->tail->next = game;
        game->prev = (*list)->tail;
        (*list)->tail = game;
        (*list)->count++;
    }
    //TODO SEND GAME ID TO PLAYER
}

void deletePlayer(playerlist_t** list, int playerId){
    player_t* current = (*list)->tail;
    while(current != NULL){
        if(current->ID == playerId){
            if(current == (*list)->head){
                (*list)->head = (*list)->head->next;
                (*list)->head->prev = NULL;
                free(current);
            }
            else if(current == (*list)->tail){
                (*list)->tail = (*list)->tail->prev;
                (*list)->tail->next = NULL;
                free(current);
            }
            else{
                current->prev->next = current->next;
                current->next->prev = current->prev;
                free(current);
            }
            return;    
        }
        current = current->next;
    }
    return;
}

void deleteGame(gamelist_t** list, int gameId){
    game_t* current = (*list)->head;
    while(current != NULL){
        if(current->gameid == gameId){
            if(current == (*list)->head){
                (*list)->head = (*list)->head->next;
                (*list)->head->prev = NULL;
                free(current);
            }
            else if(current == (*list)->tail){
                (*list)->tail = (*list)->tail->prev;
                (*list)->tail->next = NULL;
                free(current);
            }
            else{
                current->prev->next = current->next;
                current->next->prev = current->prev;
                free(current);
            }
            return;
        }
        current = current->next;
    }
    return;
}

void printPlayerlist(playerlist_t** list){
    player_t* current = (*list)->head;
    while(current != NULL){
        printf("playerId: %d\n", current->ID);
        current = current->next;
    }
    return;
}

void printGamelist(gamelist_t** list){
    game_t* current = (*list)->head;
    while(current != NULL){
        printf("gameId: %d\n", current->gameid);
        current = current->next;
    }
    return;
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