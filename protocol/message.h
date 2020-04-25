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
        
            //suka blyat
        case 1:
            printf("lmao\n");
            data = (up_pt*)(message->payload);
            printf("playerId %d", data->playerID);
            printf("gameId %d", data->playerID);
            printf("keypress x: %d", data->action.x);
            printf("keypress y: %d", data->action.y);
            printf("lmao\n");
            break;

            //LIST_GAMES
        case 2:
            //if (gameID == NULL){
            //  printf ("pis sūdu, nebūs tev NFS: Linux Terminal");
            //}   
            //for (int i = 0; i < numberOfGames; i++){
            //  printf(gameId);
            //} gameId != NULL) printf (GameID: %d, gameid);
            break;

            //GAME_INFO
        case 3:
            //printf("gameId %d", data->playerID);
                // int ID; /*Spēlētāja ID*/
                // int gameID;
                // char name[30];/* Spēlētāja vārds */

            break;

            //GET_NUMBER_OF_FIELDS
        case 4:
            //BULLSHIT_ZINJA
            //bet basically, count ar funckijām, kas prot izveidot kartes. 
            break;

            //FIELD INFO
        case 5:
            // StartLine: struct Line (skatīt protocol.h)
            // wallLine: struct Line;
                //būtu baigi dope, ja varētu atsūtīt struct ar līnijām,  bet pa lielam atsūta vektorus, ar kuriem izveido karti.

            break;

            //CREATE GAME
        case 6:
            // printw (GameID: int* Izveidotās spēles identifikators *
            // PlayerID: int* Spēles izveidotāja identifikators *
            // PlayerPass[10]: * Spēlētāja identitātes apliecinoša simbolu virkne *
            break;
        case 7: //ping
            reply.type = PONG;
            int length = ((void*)&reply.payload - (void*)&reply.type);
            sendData(fd, (void*)&reply, length, NULL);
            break


        //JOIN GAME
        // Type: “JG”
        // PlayerID: int * Tavs spēlētāja identifikators *
        // PlayerPass[10]; * Spēlētāja identitātes apliecinoša simbolu virkne *
            //no idea how these session keys work in C, but basically cookies
            //parādās fnckijas, kas pievieno spēlētaju spēlei, beigās.


        //NOTIFY_PLAYER_JOINED
        // Type: “NF”
        // PlayerID: int* Spēlētāja ID, kurš tikko pievienojās *
        // PlayerName[30];* Spēlētāja vārds *
            //parādās pēc join game izsaukuma, bet to triggero otram spēlētajam



        //START_GAME
        // Type:”SG”
        // PlayerInfoLength: size_t * PlayerInfo saraksta garums *
        // Player_info_1: struct Player_info;
        // ……………..
        // Player_info_n: struct Player_info
        // Field: struct Field;
                                    // StartLine: struct Line (skatīt protocol.h)
                                    // MainLine: struct Line
                                    // NumberOfExtraLines: int;
                    //Šie ir pointlress    // ExtraLine_0: struct Line;
                                    // ……………
                                    // ExtraLine_1: struct Line;
                //basically, nomaina game status, 
                // nosūta spēlētājiem karti un izveido spēlētājus


                //#### UDP ####
        //UPDATE_PLAYER,
        // Type: “UP”
        // GameID: int *spēles ID*
        // PlayerInfoLength: size_t * PlayerInfo saraksta garums *
        // Player_Info_1: struct Player_info; ( skatīt protocol.h)
        // ……….
        // Player_info_n: struct Player_info; 

            //game->gameID
            //for (iterator it; player -> playerID != null; it++){
            //  //send player info
            //}
            //player info 

                // int ID; /*Spēlētāja ID*/
                // int gameID;
                // char name[30];/* Spēlētāja vārds */
                // struct Coordinates position; /* Spēlētāja koordinātas */
                // float angle; /* Spēlētāja leņķis */
                // float speed; /*Spēlētāja ātrums*/
                // float acceleration; /*Spēlētāja paātrinājums*/
                // int laps; /* Cik reizes spēlētājs jau apbraucis apkārt kartei */
                // player_t* next;
                // player_t* prev;





        //END GAME
        // Type: “EG”
        // WinnerPlayerID: int *Spēlētāja ID*,
        // GameID: int *spēles ID*
                //yeslol



        case 8:
            printf("PONG\n");
            break;
        default:
            perror("invalid message");
    }
}

#endif