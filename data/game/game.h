
#include <stdio.h>
#include <stdlib.h>
#include "../player/player.h"
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

typedef struct Game{
    int gameid;
    int status; //(0- WAITING_PLAYERS, 1- STARTED, -1- ENDED);
    int hostId;
    playerlist_t* playerlist;
    struct Game* next;
    struct Game* prev;
}game_t;

typedef struct Game_list{
    int count;
    game_t* head;
    game_t* tail;
}gamelist_t;

void gamelistPush(gamelist_t** list, game_t** game){
    if((*list)->head == NULL){
        (*list)->head = *game;
        (*list)->tail = *game;
        (*list)->count++;
    }
    else{
        (*list)->tail->next = *game;
        (*game)->prev = (*list)->tail;
        (*list)->tail = *game;
        (*list)->count++;
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

// int servergamelistPush(gamelist_t** list){
//     game_t* game = (game_t*)malloc(sizeof(game_t));
//     game->playerlist = malloc(sizeof(playerlist_t));
//     game->hostId = 1;
//     playerlistPush(&game->playerlist);
//     if((*list)->head == NULL){
//         game->gameid = 1;
//         game->prev = (*list)->tail;
//         (*list)->head = game;
//         (*list)->tail = game;
//         (*list)->count++;
//     }
//     else{
//         game->gameid = (*list)->tail->gameid + 1;
//         (*list)->tail->next = game;
//         game->prev = (*list)->tail;
//         (*list)->tail = game;
//         (*list)->count++;
//     }
//     return game->gameid;
//     //TODO SEND GAME ID TO PLAYER
// }



#endif