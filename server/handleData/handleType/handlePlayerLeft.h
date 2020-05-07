#ifndef PLAYERLEFT_H_INCLUDED
#define PLAYER_H_INCLUDED

void handlePlayerLeft(msg_t* message, gamelist_t** list, playerlist_t** plist, int clientFd){
    gamelist_t* gameList = *list;
    playerlist_t* playerList = *plist;
    cg_pt* playerData = (cg_pt*)message->payload;
    game_t* currentGame = gameList->head;
    
    while(currentGame != NULL){
        if(currentGame->gameid == playerData->gameID){
            break;
        }
        currentGame = currentGame->next;
    }
    currentPlayer = currentGame->playerList->head;
    while(currentPlayer != NULL){
        if(currentPlayer->ID == playerData->playerID){
            removePlayer(&currentGame->playerList, currentPlayer->ID)
            currentPlayer->gameID = 0;
            playerlistPush(&playerList, &currentPlayer);
            if(currentGame->playerList->count == 0){
                deleteGame(&gameList, currentGame->gameid);
                return;
            }
            break;
        }
        currentPlayer = currentPlayer->next;
    }

    msg_t reply;
    reply.type = PLAYER_LEFT;
    cg_pt payload;
    payload.playerID = currentPlayer->ID;
    memcpy((void*)&reply.payload, (void*)&payload, sizeof(payload));
    int length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(payload);
    player_t* current = currentPlayer->playerList->head;
    while(current != NULL){
        sendData(current->ID, (void*)&playerUpdate, length, NULL);
        current = current->next;
    }
}
#endif