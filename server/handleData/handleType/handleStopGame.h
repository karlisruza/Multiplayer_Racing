#ifndef HANDLESTOPGAME_H_INCLUDED
#define HANDLESTOPGAME_H_INCLUDED

void handleStopGame(gamelist_t** glist, playerlist_t** plist, player_t* winner){
    gamelist_t* gameList = *glist;
    playerlist_t* playerList = *plist;

    game_t* currentGame = gameList->head;

    //get game
    while(currentGame != NULL){
        if(currentGame->gameid == winner->gameID){
            currentGame->status = -1;
            break;
        }
        currentGame = currentGame->next;
    }

    //Create stop message with winner id
    msg_t reply;
    reply.type = STOP_GAME;
    cg_pt payload;
    payload->playerID = winner->ID;
    payload->gameID = winner->gameID;
    memcpy((void*)&reply.payload, (void*)&payload, sizeof(payload));
    int length = (void*)&reply.payload - (void*)&reply.type + sizeof(payload);
    
    //send STOP_GAME to all
    player_t* currentPlayer = currentGame->playerList->head;
    while(currentPlayer != NULL){
        sendData(currentPlayer->ID, (void*)&reply, length, NULL);
        currentPlayer = currentPlayer->next;
    }

    currentPlayer = currentGame->playerList->head;
    while(currentPlayer != NULL){
            removePlayer(&currentGame->playerList, currentPlayer->ID);
            playerlistPush(&playerList, &currentPlayer);
    }

    return;
}

#endif