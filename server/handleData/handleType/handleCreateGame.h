#ifndef CREATEGAME_H_INCLUDED
#define CREATEGAME_H_INCLUDED

void handleCreateGame(gamelist_t** list, int clientFd){
    printf("Creating game...\n");
    gamelist_t* gameList = *list;

    game_t* game = (game_t*)malloc(sizeof(game_t));
    game->gameid = gameList->count++;
    game->hostId = clientFd;
    game->status = 0;
    game->playerList = (playerlist_t*)malloc(sizeof(playerlist_t));
    gamelistPush(&gameList, &game);

    msg_t reply;
    reply.type = CREATE_GAME;

    cg_pt gameinfo;
    gameinfo.gameID = game->gameid;
    gameinfo.playerID = clientFd;

    printf("gameinfo.gameid: %d\n", gameinfo.gameID);

    memcpy((void*)&reply.payload, (void*)&gameinfo, sizeof(gameinfo));
    int length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(gameinfo);
    sendData(clientFd, (void*)&reply, length, NULL);
}
#endif