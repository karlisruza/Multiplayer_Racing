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

    if(gameList != NULL){
        if(gameList->head != NULL){
            current = gameList->head;
            while(current != NULL){
                printf("current gameId:%d\n", current->gameid);
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

    if(playerList->head == NULL){
        rl_pt player;
        player.playerCount = playerList->count;
        memcpy((void*)&reply.payload, (void*)&player, sizeof(player));
        length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(player);
        sendData(clientFd, (void*)&reply, length, NULL);
    }
    else{
        player_t* current = playerList->head;
        rl_pt player;
        while(current != NULL){
            player.ID = current->ID;
            player.name = current->name;
            player.playerCount = playerList->count;

            memcpy((void*)&reply.payload, (void*)&player, sizeof(player));
            length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(player);
            sendData(clientFd, (void*)&reply, length, NULL);
            current = current->next;
        }
    }
    return;
}

#endif // !