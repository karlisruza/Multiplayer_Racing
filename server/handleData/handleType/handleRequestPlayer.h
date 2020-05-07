#ifndef HANDLEREQUESTPLAYER_H_INCLUDED
#define HANDLEREQUESTPLAYER_H_INCLUDED

void handleRequestPlayer(msg_t* message, gamelist_t** list, int clientFd){
    msg_t reply;
    reply.type = REQUEST_PLAYER;
    int length;
    gamelist_t* gameList = *list;
    playerlist_t* playerList;
    game_t* current;
    cg_pt* gameData = (cg_pt*)message->payload;

    printf("req_player gameId: %d\n", gameData->gameID);

    //checks if gameslist empty
    if(gameList != NULL){
        if(gameList->head != NULL){
            current = gameList->head;
            while(current != NULL){
                if(current->gameid == gameData->gameID){
                    playerList = current->playerList;
                    break;
                }
                current = current->next;
            }
            if(current == NULL){
                perror("requested game doesn't exist");
                return;
            }
        }
    }
    printf("\n\nhead is %d, %s\n", playerList->head->ID, playerList->head->name);

    if(playerList->head == NULL){ //checks if playerlist empty
        printf("list is empty before sending\n");
        rl_pt player;
        player.playerCount = playerList->count;
        memcpy((void*)&reply.payload, (void*)&player, sizeof(player));
        length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(player);
        sendData(clientFd, (void*)&reply, length, NULL);
    }
    else{ //send each individual player
        printf("sending indiv players\n");
        player_t* current = playerList->head;
        rl_pt player;

        int counter = 0;

        while(current != NULL){
            printf("counter - %d, user - %d ; id  %s \n",counter, current->ID, current->name);

            player.ID = current->ID;
            strcpy(player.name, current->name);
            player.playerCount = playerList->count;
            memcpy((void*)&reply.payload, (void*)&player, sizeof(player));
            length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(player);
            sendData(clientFd, (void*)&reply, length, NULL);
            current = current->next;
            counter++;
        }
    }
    return;
}

#endif // !