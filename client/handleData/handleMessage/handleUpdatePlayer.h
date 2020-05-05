#ifndef HANDLEUPDATEPLAYER_H_INCLUDED
#define HANDLEUPDATEPLAYER_H_INCLUDED

int handleUpdatePlayer(playerlist_t** list, player_t* clientPlayer, int clientFd){
    void* buffer = (void*)malloc(sizeof(msg_t));
    playerlist_t* playerList = *list;
    updpos_pt* playerData;
    int playerCount;
    int counter = 0;

    do{
        int ret =  recv(clientFd, buffer, sizeof(msg_t) - MAX_PAYLOAD_SIZE + sizeof(updpos_pt), 0);
        msg_t* reply = (msg_t*)buffer;
        if(ret < 0 || reply->type != REQUEST_PLAYER){
            perror("REQUEST_PLAYER invalid response");
            return -1;
        }
        playerData = (updpos_pt*)reply->payload;
        playerCount = playerData->playerCount;
        if(playerCount == 0){
            return 1;
        }
        
        player_t* current = playerList->head;
        while(current != NULL){
            if(current->ID = playerData->ID){
                current->position.x = playerData->x;
                current->position.y = playerData->y;
                current->angle = playerData->angle;
            }
            current = current->next;
        }
        counter++;
    }while(counter < playerCount);

}

#endif