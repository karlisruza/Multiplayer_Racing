#ifndef HANDLEJOINGAME_H_INCLUDED
#define HANDLEJOINGAME_H_INCLUDED

#define START_ANGLE 0;
#define START_X 20;
#define START_Y 120;

void handleJoinGame(msg_t* message, gamelist_t** glist, playerlist_t** plist, int clientFd){
    cg_pt* joinData = (cg_pt*)message->payload;
    gamelist_t* gameList = *glist;
    playerlist_t* playerList = *plist;
    game_t* currentGame;
    
    //checks if list exists and isnt empty;
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

    //finds the game to join to
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

    //remove player from playerList and add to games' playerlist
    player_t* current = playerList->head;
    int counter = 0; //for start_y offset
    while(current != NULL){
        if(current->ID == clientFd){
            //remove from playerList and push in to the games playerlist
            removePlayer(&playerList, clientFd);
            //set start coords and angle;
            current->gameID = currentGame->gameid;
            current->position.x = START_X;
            current->position.y = START_Y + counter * 2;
            current->angle = START_ANGLE;
            current->speed = 0;
            current->acceleration = 0;
            playerlistPush(&currentGame->playerList, &current);
            break;
        }
        else{
            perror("The game you wanted to join doesn't exist");
            return;
        }
        current = current->next;
        counter++;
    }

    //confirmation reply
    msg_t reply;
    reply.type = JOIN_GAME;
    int length = ((void*)&reply.payload - (void*)&reply.type);
    sendData(clientFd, (void*)&reply, length, NULL);

    //send new player to all
    rl_pt player;
    msg_t playerUpdate;
    playerUpdate.type = PLAYER_JOINED;
    player.ID = clientFd;
    strcpy(player.name, current->name);
    player.playerCount = 1;

    memcpy((void*)&playerUpdate.payload, (void*)&player, sizeof(player));
    length = ((void*)&playerUpdate.payload - (void*)&playerUpdate.type) + sizeof(player);

    current = currentGame->playerList->head;
    //exclude player who just joined and send to all in lobby
    while(current != NULL){
        if(current->ID != player.ID){
            sendData(current->ID, (void*)&playerUpdate, length, NULL);
        }
        current = current->next;
    }

    return; 
}

#endif // !
