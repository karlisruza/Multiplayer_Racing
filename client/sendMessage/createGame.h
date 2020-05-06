#ifndef CREATEGAME_H_INCLUDED
#define CREATEGAME_H_INCLUDED

int createGame(player_t** player, int clientFd){
    msg_t request;
    request.type = CREATE_GAME;
    void* buffer = (void*)malloc(sizeof(msg_t));
    int length = ((void*)&request.payload - (void*)&request.type);
    sendData(clientFd, (void*)&request, length, NULL);

    recv(clientFd, buffer, sizeof(msg_t), 0);
    msg_t* reply = (msg_t*)buffer;
    if(reply->type != CREATE_GAME){
        perror("Received msg of incorrect type");
        return -1;
    }
    cg_pt* gameData = (cg_pt*)reply->payload;
    (*player)->gameID= gameData->gameID;
    return 0;
}

#endif