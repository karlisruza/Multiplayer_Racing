#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "./protocol.h"
#include "./payload.h"
#include "./racinglist.h"
#include "./listtest.h"

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
    REQUEST_GAME = 9,
    CREATE_GAME = 10,
    RECEIVE_GAME = 11,
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

void handleDataS(msg_t *message, params_t* params){
    msg_t reply;
    up_pt* data;
    gamelist_t* gameslist;
    int fd = params->clientFd;
    gameslist = params->list;
    int length;

    printf("aaaa \n");
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
            //join game
            
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
            length = ((void*)&reply.payload - (void*)&reply.type);
            sendData(fd, (void*)&reply, length, NULL);
            break;
        case 8:
            printf("PONG\n");
            break;
        case 9:
            reply.type = REQUEST_GAME;
            if(gameslist->head == NULL){
                reply.payload[0] = '0';
                length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(char);
                sendData(fd, (void*)&reply, length, NULL);
            }
            else{
                game_t* current = gameslist->head;
                rg_pt game;
                while(current != NULL){
                    game.gameId = current->gameid;
                    game.hostId = current->playerlist->head->ID;
                    game.gamecount = gameslist->count;
                    game.status = current->status;

                    printGame(current);

                    memcpy((void*)&reply.payload, (void*)&game, sizeof(game));
                    length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(game);
                    sendData(fd, (void*)&reply, length, NULL);
                    sleep(1);
                    current = current->next;
                }
                return;
            }
            break;
        case 10:
            printf("Creating game...\n");
            int gameId = gamelistPush(&gameslist);
            reply.type = CREATE_GAME;

            cg_pt gameinfo;
            gameinfo.gameID = gameId;
            gameinfo.playerID = 1;

            printf("gameinfo.gameid: %d\n", gameinfo.gameID);

            memcpy((void*)&reply.payload, (void*)&gameinfo, sizeof(gameinfo));
            length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(gameinfo);
            sendData(fd, (void*)&reply, length, NULL);
            break;
        default:
            perror("invalid message");
    }
}

void handleDataC(msg_t *message, int fd, gamelist_t** gamelist){
    msg_t reply;
    up_pt* data;
    cg_pt* gameinfo;
    int length;

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
            length = ((void*)&reply.payload - (void*)&reply.type);
            sendData(fd, (void*)&reply, length, NULL);
            break;
        case 8:
            printf("PONG\n");
            break;
        case 9:{
            rg_pt* gamedata = (rg_pt*)message->payload;
            gamelist_t* list = *gamelist;
            
            printf("gameCount: %d//////\n", gamedata->gamecount);
            int gamecount = gamedata->gamecount;
            char* buffer = malloc(sizeof(MAX_PAYLOAD_SIZE));
            printf("buffersize: %ld\n", sizeof(buffer));

            for(int i = 0; i < gamecount; i++){
                if(i != 0){
                    printf("pre ret\n");
                    int ret =  recv(fd, buffer, sizeof(msg_t) + MAX_PAYLOAD_SIZE, 0);
                    printf("post ret\n");
                    printf("buffersize: %ld\n", sizeof(buffer));

                    message = (msg_t*)buffer;
                }

                gamedata = (rg_pt*)message->payload;
                game_t* game = malloc(sizeof(game_t));
                game->gameid = gamedata->gameId;
                game->hostId = gamedata->hostId;
                game->status = gamedata->status;
                game->playerlist = NULL;

                printGame(game);
                clientgamelistPush(&list, &game);
                free(message);

                printf("i: %d\n", i);
            }
            break;
        }
        case 10:{
            gameinfo = (cg_pt*)message->payload;
            printf("gameId: %d\n", gameinfo->gameID);
            printf("gameId: %d\n", gameinfo->playerID);
            printf("case 11\n");
            break;
        }
        case 11:{


        }
        default:
            perror("invalid message");
    }
}

void printClient(int fd) {
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    if (getpeername(fd, (struct sockaddr *) &addr, &addrLen) == -1) {
        perror("getpeername");
        return;
    }
    printf("%s:%d connected via TCP\n",
	inet_ntoa(addr.sin_addr),
	ntohs(addr.sin_port));
}

#endif