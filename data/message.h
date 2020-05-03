#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "./protocol.h"
#include "./payload/payload.h"
#include "./game/game.h"
#include "./constant.h"
#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

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
    REQUEST_GAME = 9,
    CREATE_GAME = 10,
    ADD_PLAYER = 11,
    REQUEST_PLAYER = 12,
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

int payloadSize(int type){
    switch(type){
        case REQUEST_GAME:
            return sizeof(rg_pt);
            break;
        case CREATE_GAME:
            return 0;
            break;
        case ADD_PLAYER:{
            return 0;
            break;
        }
        case REQUEST_PLAYER:
            return sizeof(rl_pt);
            break;
        case JOIN_GAME:{
            return 0;
            break;
        }
        default:
            return 0;
    }
    return 0;
}

#endif