#include <stdio.h>
#include <stdlib.h>
#include "./protocol.h"
#include "./racinglist.h"

#ifndef LISTTEST_H_INCLUDED
#define LISTTEST_H_INCLUDED

void printPlayer(player_t* player){
    printf("    Player ID: %d\n", player->ID);
    printf("    gameID: %d\n", player->ID);
    printf("    name: %c\n", player->ID);
    printf("    position x: %f\n", player->position.x);
    printf("    position y: %f\n", player->position.y);
    printf("    speed: %f\n", player->speed);
    printf("    acceleration: %f\n", player->acceleration);
    printf("    laps: %d\n", player->laps);
    printf("\n");
    return;
}

void printPlayerlist(playerlist_t** list){
    player_t* current = (*list)->head;
    while(current != NULL){
        printPlayer(current);
        current = current->next;
    }
    return;
}

void printGame(game_t* game){
    printf("Game ID: %d\n", game->gameid);
    printf("Status: %d\n", game->status);
    if(game->playerlist != NULL){
        printf("Player Count: %d\n", game->playerlist->count);
        printPlayerlist(&game->playerlist);  
    } 
}

void printGamelist(gamelist_t** list){
    game_t* current = (*list)->head;
    while(current != NULL){
        printGame(current);
        current = current->next;
    }
    return;
}

void getTestingList(gamelist_t** list){
    gamelist_t* gamelist = *list;

    gamelistPush(&gamelist);
    gamelistPush(&gamelist);
    gamelistPush(&gamelist);
    gamelistPush(&gamelist);
    gamelistPush(&gamelist);

    playerlistPush(&gamelist->head->playerlist);
    playerlistPush(&gamelist->head->playerlist);
    playerlistPush(&gamelist->head->playerlist);
    playerlistPush(&gamelist->head->playerlist);
    playerlistPush(&gamelist->head->playerlist);

    playerlistPush(&gamelist->head->next->playerlist);
    playerlistPush(&gamelist->head->next->playerlist);
    playerlistPush(&gamelist->head->next->playerlist);
    playerlistPush(&gamelist->head->next->playerlist);

    playerlistPush(&gamelist->head->next->next->playerlist);
    playerlistPush(&gamelist->head->next->next->playerlist);
    playerlistPush(&gamelist->head->next->next->playerlist);

}

#endif
