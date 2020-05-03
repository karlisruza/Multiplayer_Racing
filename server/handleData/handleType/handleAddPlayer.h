#ifndef HANDLEADDPLAYER_H_INCLUDED
#define HANDLEADDPLAYER_H_INCLUDED

void handleAddPlayer(msg_t* message, int clientFd, playerlist_t** list){
    msg_t reply;
    int length;
    char* playername[30]; 
    strcpy((char*)message->payload, &playername);
    player_t* current;
    playerlist_t* playerList = *list;
    printf("pname: %s\n", playername);

    if(playerList != NULL && playerList->head != NULL){
        current = playerList->head;
    }
    else{
        perror("handleAddPlayer invalid list");
        return;
    }
    //set player name to received name in playerList
    while(current != NULL){
        if(current->ID == clientFd){
            current->name = playername;
        }
        current=current->next;
    }

    //send back player Id 
    reply.type = ADD_PLAYER;
    cg_pt data;
    data.playerID = clientFd;
    data.gameID = 0;

    memcpy((void*)&reply.payload, (void*)&data, sizeof(data));
    length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(data);
    sendData(clientFd, (void*)&reply, length, NULL);
    return;
}

#endif