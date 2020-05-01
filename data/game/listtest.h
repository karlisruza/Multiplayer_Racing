#include <stdio.h>
#include <stdlib.h>
#include "./game.h"

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

void printPlayerList(playerlist_t** list){
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
        printPlayerList(&game->playerlist);  
    } 
}

void printGameList(gamelist_t** list){
    game_t* current = (*list)->head;
    while(current != NULL){
        printGame(current);
        current = current->next;
    }
    return;
}

void getTestingList(gamelist_t** list){
    gamelist_t* gamelist = *list;
    
    game_t* game = (game_t*)malloc(sizeof(game_t));
    game->gameid = 1;
    game->hostId = 0;
    gamelistPush(&gamelist, &game);

    game = (game_t*)malloc(sizeof(game_t));
    game->gameid = 2;
    game->hostId = 0;
    gamelistPush(&gamelist, &game);

    game = (game_t*)malloc(sizeof(game_t));
    game->gameid = 3;
    game->hostId = 0;
    gamelistPush(&gamelist, &game);

    game = (game_t*)malloc(sizeof(game_t));
    game->gameid = 4;
    game->hostId = 0;
    gamelistPush(&gamelist, &game);

    game = (game_t*)malloc(sizeof(game_t));
    game->gameid = 5;
    game->hostId = 0;
    gamelistPush(&gamelist, &game);

    // gamelistPush(&gamelist->head->playerlist);
    // gamelistPush(&gamelist->head->playerlist);
    // gamelistPush(&gamelist->head->playerlist);
    // gamelistPush(&gamelist->head->playerlist);
    // gamelistPush(&gamelist->head->playerlist);

    // gamelistPush(&gamelist->head->next->playerlist);
    // gamelistPush(&gamelist->head->next->playerlist);
    // gamelistPush(&gamelist->head->next->playerlist);
    // gamelistPush(&gamelist->head->next->playerlist);

    // gamelistPush(&gamelist->head->next->next->playerlist);
    // gamelistPush(&gamelist->head->next->next->playerlist);
    // gamelistPush(&gamelist->head->next->next->playerlist);

}

#endif
