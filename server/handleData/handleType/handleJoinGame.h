#ifndef HANDLEJOINGAME_H_INCLUDED
#define HANDLEJOINGAME_H_INCLUDED

void handleJoinGame(msg_t* message, gamelist_t** glist, playerlist_t** plist, int clientFd){
    cg_pt* joinData = (cg_pt*)message->payload;
    gamelist_t* gameList = *glist;
    playerlist_t* playerList = *plist;
    game_t* currentGame;
    player_t* currentPlayer;
    
    //checks valid lists
    if(gameList != NULL && playerList != NULL){
        if(playerList->head != NULL && gameList->head != NULL){
            currentPlayer = playerList->head;
            currentGame = gameList->head;
        }
        else{
            perror("The game you wanted to join doesn't exist");
            return;
        }
    }
    else{
        perror("The game you wanted to join doesn't exist");
        return;
    }


    while(currentPlayer != NULL){
        if(currentPlayer->ID == joinData->playerID){
            break;
        }
        currentPlayer = currentPlayer->next;
    }
    while(currentGame != NULL){
        if(currentGame->gameid == joinData->gameID){
            break;
        }
        currentGame = currentGame->next;
    }
    if(currentGame == NULL || currentPlayer == NULL){
        perror("Failed to join game");
        return;
    }
    printf("loopsdone\n");

    msg_t reply;
    reply.type = JOIN_GAME;
    int length = ((void*)&reply.payload - (void*)&reply.type);
    sendData(clientFd, (void*)&reply, length, NULL);

    printf("hello from join game\n");
    playerlistPush(&currentGame->playerList, &currentPlayer);
    printGameById(&gameList, joinData->gameID);
    return; 
}

#endif // !
