#include <stdio.h>
#include <stdlib.h>
#include "./game.h"

#ifndef LISTTEST_H_INCLUDED
#define LISTTEST_H_INCLUDED

void getTestingList(gamelist_t** list){
    gamelist_t* gamelist = *list;
    	
//game with two players
	game_t* game = (game_t*)malloc(sizeof(game_t));
    game->gameid = 1;
    game->hostId = 4;
	game->playerList = (playerlist_t*)malloc(sizeof(playerlist_t));
    gamelistPush(&gamelist, &game);

	player_t* player = (player_t*)malloc(sizeof(player_t*));
	strcpy(player->name, "pleb");
	player->ID = 2;
	player->gameID = 1;
	player->x = 77.0;
	player->y = 99.0;
	player->acceleration = 0.0000001;
	player->speed = 1.0;
	playerlistPush(&game->playerList, &player);

	player = (player_t*)malloc(sizeof(player_t*));
	strcpy(player->name, "pleb");
	player->ID = 2;
	player->gameID = 1;
	player->x = 77.0;
	player->y = 99.0;
	player->acceleration = 0.0000001;
	player->speed = 1.0;
	playerlistPush(&game->playerList, &player);
	
	player = (player_t*)malloc(sizeof(player_t*));
	strcpy(player->name, "pleb");
	player->ID = 2;
	player->gameID = 1;
	player->x = 77.0;
	player->y = 99.0;
	player->acceleration = 0.0000001;
	player->speed = 1.0;
	playerlistPush(&game->playerList, &player);
	return;

//game with one player 

}



#endif
