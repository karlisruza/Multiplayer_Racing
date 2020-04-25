#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "./protocol.h"
#include "./payload.h"

#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#define MAX_PAYLOAD_SIZE 128


typedef enum{
    //messages sent by client
    UP = 1, 
    //sends current player data
    //Payload contents
    //int playerID;
    //int gameID;
    //action_t action

    JOIN_GAME = 2,

    //messages sent by server
    START_GAME = 3,
    STOP_GAME = 4,
    UPS = 5, //update all players
    EG = 6, //end game
    //messages sent by both
    PING = 7,
    PONG = 8,
}mtype_e;

typedef struct msg_s{
    mtype_e type;
    char payload[MAX_PAYLOAD_SIZE];
}msg_t;

void sendData(int fd, char *data, unsigned length, struct sockaddr_in *remote) {
    int ret;
    if (remote != NULL) {
	    ret = sendto(fd, data, length, 0, (struct sockaddr *) remote, sizeof(remote));
    } else {
	    ret = send(fd, data, length, 0);
    }
    if (ret < 0) {
        printf(":(\n");
        perror("send");
        return;
    }
}

void handleData(msg_t *message, int fd){
    msg_t reply;
    up_pt* data;

    printf("handleData enum %d\n", message->type);
    switch (message->type){
        case 1:
            printf("lmao\n");
            data = (up_pt*)(message->payload);
            printf("playerId %d", data->playerID);
            printf("gameId %d", data->playerID);
            printf("keypress x: %d", data->action.x);
            printf("keypress y: %d", data->action.y);
            printf("lmao\n");
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7: //ping
            reply.type = PONG;
            int length = ((void*)&reply.payload - (void*)&reply.type);
            sendData(fd, (void*)&reply, length, NULL);
            break;
        case 8:
            printf("PONG\n");
            break;
        default:
            perror("invalid message");
    }
}

#endif