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
    while(current != NULL){
        printGame(current);
        current = current->next;
    }
    return;
}

void getTestingList(gamelist_t** list){
    gamelist_t* gamelist = *list;
    	
//game with two players
	    game_t* game = (game_t*)malloc(sizeof(game_t));
	    game->gameid = 1;
	    game->hostId = 0;
	    gamelistPush(&gamelist, &game);

	    player_t* player = (player_t*)malloc(sizeof(player_t*));
	    player->name = "hostplayer";
	    player->ID = 1;
	    player->gameID = 1;
	    playerlistPush(&game->playerList, &player);

	    player = (player_t*)malloc(sizeof(player_t*));
	    player->name = "pleb";
	    player->ID = 2;
	    player->gameID = 1;
	    playerlistPush(&game->playerList, &player);

//game with one player 

	    game = (game_t*)malloc(sizeof(game_t));
	    game->gameid = 2;
	    game->hostId = 3;
	    gamelistPush(&gamelist, &game);

	    player = (player_t*)malloc(sizeof(player_t*));
	    player->name = "xXkAArLizXx123";
	    player->ID = 3;
	    player->gameID = 2;
	    playerlistPush(&game->playerList, &player);


//game with no players
	    game = (game_t*)malloc(sizeof(game_t));
	    game->gameid = 3;
	    game->hostId = 0;
	    gamelistPush(&gamelist, &game);

//game with many players
	    game = (game_t*)malloc(sizeof(game_t));
	    game->gameid = 4;
	    game->hostId = 6;
	    game->playerList = (playerlist_t*)malloc(sizeof(playerlist_t));
	    gamelistPush(&gamelist, &game);
	
	for (int i = 0; i < 8; i++){
	    player = (player_t*)malloc(sizeof(player_t*));
	    player->name = "mrAnderson";
	    player->ID = i+4;
	    player->gameID = 4;
	    playerlistPush(&game->playerList, &player);
	}
		

	for (int i = 0; i < 3; i++){
	    player = (player_t*)malloc(sizeof(player_t*));
	    player->name = "nobody";
	    player->ID = i*15;
	    player->gameID = 0;
	    playerlistPush(&game->playerList, &player);
	}


	//game that has no host and/or is completed.
	    game = (game_t*)malloc(sizeof(game_t));
	    game->gameid = 5;
	    game->hostId = 0;
	    gamelistPush(&gamelist, &game);

    // gamelistPush(&gamelist->head->playerLis);
    // gamelistPush(&gamelist->head->playerLis);
    // gamelistPush(&gamelist->head->playerLis);
    // gamelistPush(&gamelist->head->playerLis);
    // gamelistPush(&gamelist->head->playerLis);

    // gamelistPush(&gamelist->head->next->playerLis);
    // gamelistPush(&gamelist->head->next->playerLis);
    // gamelistPush(&gamelist->head->next->playerLis);
    // gamelistPush(&gamelist->head->next->playerLis);

    // gamelistPush(&gamelist->head->next->next->playerLis);
    // gamelistPush(&gamelist->head->next->next->playerLis);
    // gamelistPush(&gamelist->head->next->next->playerLis);

}



#endif
