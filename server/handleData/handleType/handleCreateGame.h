#ifndef CREATEGAME_H_INCLUDED
#define CREATEGAME_H_INCLUDED

void handleCreateGame(gamelist_t** list, playerlist_t** plist, int clientFd){
    printf("Creating game...\n");
    gamelist_t* gameList = *list;
    playerlist_t* playerList = *plist;

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

    memcpy((void*)&reply.payload, (void*)&gameinfo, sizeof(gameinfo));
    int length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(gameinfo);
    sendData(clientFd, (void*)&reply, length, NULL);
}
#endif