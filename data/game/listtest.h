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
    game->hostId = 0;
    gamelistPush(&gamelist, &game);

	    player_t* player = (player_t*)malloc(sizeof(player_t*));
	    strcpy(player->name, "hostplayer");
	    player->ID = 1;
	    player->gameID = 1;
	    playerlistPush(&game->playerList, &player);

	    player = (player_t*)malloc(sizeof(player_t*));
	    strcpy(player->name, "pleb");
	    player->ID = 2;
	    player->gameID = 1;
	    playerlistPush(&game->playerList, &player);

//game with one player 

    player_t* host = (player_t*)malloc(sizeof(player_t*));
    char* Name = "hostplayer99";
	strcpy(host->name, Name);
    host->ID = 99;
    host->gameID = 4;
    playerlistPush(&game->playerList, &host);

    game = (game_t*)malloc(sizeof(game_t));
    game->gameid = 5;
    game->hostId = 0;
    gamelistPush(&gamelist, &game);
//game with one player 

	    // game = (game_t*)malloc(sizeof(game_t));
	    // game->gameid = 2;
	    // game->hostId = 3;
	    // gamelistPush(&gamelist, &game);

	    // player = (player_t*)malloc(sizeof(player_t*));
	    // player->name = "xXkAArLizXx123";
	    // player->ID = 3;
	    // player->gameID = 2;
	    // playerlistPush(&game->playerList, &player);


// //game with no players
// 	    game = (game_t*)malloc(sizeof(game_t));
// 	    game->gameid = 3;
// 	    game->hostId = 0;
// 	    gamelistPush(&gamelist, &game);

//game with many players
	//     game = (game_t*)malloc(sizeof(game_t));
	//     game->gameid = 4;
	//     game->hostId = 6;
	//     game->playerList = (playerlist_t*)malloc(sizeof(playerlist_t));
	//     gamelistPush(&gamelist, &game);
	
	// for (int i = 0; i < 8; i++){
	//     player = (player_t*)malloc(sizeof(player_t*));
	//     player->name = "mrAnderson";
	//     player->ID = i+4;
	//     player->gameID = 4;
	//     playerlistPush(&game->playerList, &player);
	// }
		

	// for (int i = 0; i < 3; i++){
	//     player = (player_t*)malloc(sizeof(player_t*));
	//     player->name = "nobody";
	//     player->ID = i*15;
	//     player->gameID = 0;
	//     playerlistPush(&game->playerList, &player);
	// }


	//game that has no host and/or is completed.
	    // game = (game_t*)malloc(sizeof(game_t));
	    // game->gameid = 5;
	    // game->hostId = 0;
	    // gamelistPush(&gamelist, &game);

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
