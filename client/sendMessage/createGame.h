#ifndef CREATEGAME_H_INCLUDED
#define CREATEGAME_H_INCLUDED

int createGame(player_t** player, int clientFd){
    msg_t request;
    request.type = CREATE_GAME;

    int length = ((void*)&request.payload - (void*)&request.type);
    sendData(clientFd, (void*)&request, length, NULL);
    printf("a1\n");
    void* buffer = (void*)malloc(sizeof(msg_t));
    recv(clientFd, buffer, sizeof(msg_t), 0);
    msg_t* reply = (msg_t*)buffer;
    if(reply->type != CREATE_GAME){
        perror("Received msg of incorrect type");
        return -1;
    }
    printf("a2\n");
    cg_pt* gameData = (cg_pt*)reply->payload;
    (*player)->gameID= gameData->gameID;
    return gameData->gameID;
}

#endif