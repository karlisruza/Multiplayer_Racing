#ifndef HANDLEJOINGAME_H_INCLUDED
#define HANDLEJOINGAME_H_INCLUDED

void handleJoinGame(msg_t* message, gamelist_t** glist, playerlist_t** plist, int clientFd){
    cg_pt* joinData = (cg_pt*)message->payload;
    gamelist_t* gameList = *glist;
    playerlist_t* playerList = *plist;
    game_t* currentGame;
    
    //checks valid lists
    if(gameList != NULL){
        if(gameList->head != NULL){
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

    while(currentGame != NULL){
        if(currentGame->gameid == joinData->gameID){
            break;
        }
        currentGame = currentGame->next;
    }
    if(currentGame == NULL){
        perror("Failed to join game");
        return;
    }

    player_t* current = playerList->head;
    while(current != NULL){
        if(current->ID = clientFd){
            removePlayer(&playerList, clientFd);
            playerlistPush(&currentGame->playerList, &current);
            printPlayerList(&currentGame->playerList);
            current->gameID = currentGame->gameid;
            break;
        }
        else{
            perror("The game you wanted to join doesn't exist");
            return;
        }
        current = current->next;
    }

    msg_t reply;
    reply.type = JOIN_GAME;
    int length = ((void*)&reply.payload - (void*)&reply.type);
    sendData(clientFd, (void*)&reply, length, NULL);
    return; 
}

#endif // !
