#ifndef HANDLEREQUESTPLAYER_H_INCLUDED
#define HANDLEREQUESTPLAYER_H_INCLUDED

void handleRequestPlayer(msg_t* message, playerlist_t** list, int clientFd){
    msg_t reply;
    reply.type = REQUEST_PLAYER;
    int length;
    playerlist_t* playerList = *list;

    if(playerList->head == NULL){
        rp_pt player;
        player.playerCount = playerList->count;
        memcpy((void*)&reply.payload, (void*)&player, sizeof(player));
        length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(player);
        sendData(clientFd, (void*)&reply, length, NULL);
    }
    else{
        player_t* current = playerList->head;
        rp_pt player;
        while(current != NULL){
            player.player = current;
            player.playerCount = playerList->count;

            memcpy((void*)&reply.payload, (void*)&player, sizeof(player));
            length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(player);
            sendData(clientFd, (void*)&reply, length, NULL);
            // sleep(1);
            current = current->next;
        }
    }
    return;
}

#endif // !