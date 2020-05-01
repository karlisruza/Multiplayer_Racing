#ifndef REQUESTPLAYER_H_INCLUDED
#define REQUESTPLAYER_H_INCLUDED

//return -1 error, 1 success
int requestPlayer(playerlist_t** list, int clientFd){
    msg_t request;
    request.type = REQUEST_PLAYER;
    int length = ((void*)&request.payload - (void*)&request.type);
    sendData(clientFd, (void*)&request, length, NULL);
    playerlist_t* playerList = *list;
    void* buffer = (void*)malloc(MAX_PAYLOAD_SIZE + sizeof(msg_t));
    rp_pt* playerData;

    int playerCount;
    int counter = 0;

    do{
        int ret =  recv(clientFd, buffer, sizeof(msg_t) - MAX_PAYLOAD_SIZE + sizeof(rp_pt), 0);
        msg_t* reply = (msg_t*)buffer;
        if(ret < 0 || reply->type != REQUEST_GAME){
            perror("REQUEST_GAME invalid response");
            return -1;
        }
        playerData = (rp_pt*)reply->payload;
        playerCount = playerData->playerCount;
        if(playerCount == 0){
            return 1;
        }
        player_t* player = playerData->player;

        playerlistPush(&playerList, &player);
        counter++;
    }while(counter < playerCount);

    return 0;
}

#endif // !REQUESTGAME_H_INCLUDED