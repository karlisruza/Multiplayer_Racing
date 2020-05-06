#include <stdlib.h>
#include <stdio.h>
#include "../protocol.h"

#ifndef PAYLOAD_H_INCLUDED
#define PAYLOAD_H_INCLUDED

//These structs define the data to be sent as the payload
typedef struct update_payload_type{
    int playerID;
    int gameID;
    action_t action;
}up_pt;

typedef struct create_game_payload_type{
    int playerID;
    int gameID;
}cg_pt;

typedef struct request_game_payload_type{
    int gameId;
    int status;
    int gamecount;
    int hostId;
}rg_pt;

typedef struct request_lobby_payload_type{
    int ID; /*Spēlētāja ID*/
    char name[20];/* Spēlētāja vārds */
    int playerCount;
}rl_pt;

typedef struct update_pos_type{
    int ID; /*Spēlētāja ID*/
    float x;
    float y;
    float angle;
}updpos_pt;




#endif