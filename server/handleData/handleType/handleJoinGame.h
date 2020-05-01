#ifndef HANDLEJOINGAME_H_INCLUDED
#define HANDLEJOINGAME_H_INCLUDED

void handleJoinGame(msg_t* message, gamelist_t** glist, playerlist_t** plist){
    cg_pt* joinData = (cg_pt*)message->payload;
    gamelist_t* gameList = *glist;
    playerlist_t* playerList = *plist;
    player_t* currentPlayer = playerList->head;
    game_t* currentGame = gameList->head;

    while(currentPlayer->ID != joinData->playerID || currentPlayer != NULL){
        currentPlayer = currentPlayer->next;
    }
    while(currentGame->gameid != joinData->gameID || currentGame != NULL){
        currentGame = currentGame->next;
    }
    if(currentGame == NULL || currentPlayer == NULL){
        perror("Failed to join game");
        return;
    }
    playerlistPush(&currentGame->playerList, &currentPlayer);
    return;
}

#endif // !
