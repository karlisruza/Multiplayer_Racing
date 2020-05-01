#include <sys/socket.h>
#include <string.h>
#include "../../data/message.h"
#include "../../data/payload/payload.h"
#include "../../data/constant.h"

#ifndef SENDNAME_H_INCLUDED
#define SENDNAME_H_INCLUDED

//sends name and receives player id as response
int sendName(char* playerName, int clientFd){
    void* buffer = malloc(MAX_PAYLOAD_SIZE + sizeof(msg_t));
    msg_t message;
    message.type = ADD_PLAYER;    
    memcpy((void*)&message.payload, (void*)playerName, strlen(playerName) + 1);
    int length = ((void*)&message.payload - (void*)&message.type) + (strlen(playerName) + 1)*sizeof(char);
    sendData(clientFd, (void*)&message, length, NULL);

    recv(clientFd, buffer, sizeof(msg_t) + MAX_PAYLOAD_SIZE, 0);
    msg_t* reply = (msg_t*)buffer;
    cg_pt* payload = (cg_pt*)reply->payload;

    return payload->playerID;
}

#endif