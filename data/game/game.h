
#include <stdio.h>
#include <stdlib.h>
#include "../player/player.h"
#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

typedef struct Game{
    int gameid;
    int status; //(0- WAITING_PLAYERS, 1- STARTED, -1- ENDED);
    int hostId;
    playerlist_t* playerList;
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

void printGame(game_t* game){
    printf("Game ID: %d\n", game->gameid);
    printf("Status: %d\n", game->status);
    if(game->playerList != NULL){
        printf("Player Count: %d\n", game->playerList->count);
        printPlayerList(&game->playerList);  
    } 
}

void printGameById(gamelist_t** list, int id){
    game_t* current = (*list)->head;
    while(current != NULL){
        if(current->gameid == id){
            printGame(current);
        }
        current = current->next;
    }
    return;
}

void printGameList(gamelist_t** list){
    game_t* current = (*list)->head;
    if((*list)->head == NULL){
        printf("gamelist empty\n");
        return;
    }
    while(current != NULL){
        printGame(current);
        current = current->next;
    }
    return;
}

void deleteGameList(gamelist_t** list){
    gamelist_t* gameList = *list;
    game_t* temp1;
    game_t* temp2;
    if(gameList->head == gameList->tail){
        free(gameList->head);
        return;
    }
    else{
        temp1 = gameList->head;
        temp2 = gameList->head->next;
        while(temp1 != NULL){
            if(temp1 == gameList->tail){
                free(temp1);
                return;
            }
            free(temp1);
            temp1 = temp2;
            temp2 = temp2->next;
        }
    }
    return;
}

#endif