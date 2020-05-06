#ifndef HANDLESTARTGAME_H_INCLUDED
#define HANDLESTARTGAME_H_INCLUDED

void handleStartGame(msg_t* message, gamelist_t** glist, int clientFd){
    cg_pt* gameData = (cg_pt*)message->payload;
    gamelist_t* gameList = *glist;
    game_t* currentGame = gameList->head;

    while(currentGame != NULL){
        if(currentGame->gameid == gameData->gameID){
            currentGame->status = 1;
            break;
        }
        currentGame = currentGame->next;
    }

    msg_t reply;
    reply.type = START_GAME;
    int length = (void*)&reply.payload - (void*)&reply.type;

    player_t* currentPlayer = currentGame->playerList->head;
    while(currentPlayer != NULL){
        if(currentPlayer->ID == clientFd){ //if just for testing
            sendData(currentPlayer->ID, (void*)&reply, length, NULL);
        }
        currentPlayer = currentPlayer->next;
    }

    return;
}

#endif