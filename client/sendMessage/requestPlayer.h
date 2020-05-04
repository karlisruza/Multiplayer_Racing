#ifndef REQUESTPLAYER_H_INCLUDED
#define REQUESTPLAYER_H_INCLUDED

//return -1 error, 1 success
int requestPlayer(playerlist_t** list, player_t** clientPlayer, int clientFd){
    msg_t request;
    request.type = REQUEST_PLAYER;
    
    cg_pt payload;
    payload.gameID = (*clientPlayer)->gameID;
    payload.playerID = (*clientPlayer)->ID;
    memcpy((void*)&request.payload, (void*)&payload, sizeof(payload));

    int length = ((void*)&request.payload - (void*)&request.type) + sizeof(payload);
    sendData(clientFd, (void*)&request, length, NULL);

    playerlist_t* playerList = *list;
    void* buffer = (void*)malloc(MAX_PAYLOAD_SIZE + sizeof(msg_t));
    rl_pt* playerData;

    int playerCount;
    int counter = 0;

    do{
        printf("rqplayer\n");
        int ret =  recv(clientFd, buffer, sizeof(msg_t) - MAX_PAYLOAD_SIZE + sizeof(rl_pt), 0);
        msg_t* reply = (msg_t*)buffer;
        if(ret < 0 || reply->type != REQUEST_PLAYER){
            perror("REQUEST_PLAYER invalid response");
            return -1;
        }
        playerData = (rl_pt*)reply->payload;
        playerCount = playerData->playerCount;
        if(playerCount == 0){
            return 1;
        }
        player_t* player = (player_t*)malloc(sizeof(player_t));
        player->ID = playerData->ID;
        player->name = playerData->name;
        player->angle = 0;
        player->laps = 0;

        playerlistPush(&playerList, &player);
        counter++;
    }while(counter < playerCount);

    return 0;
}

#endif // !REQUESTGAME_H_INCLUDED