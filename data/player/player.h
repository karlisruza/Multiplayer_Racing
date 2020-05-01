#include <stdio.h>
#include <stdlib.h>
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

typedef struct Coordinates{
    float x;
    float y;
}coordinate_t;

typedef struct Player_info{
    int ID; /*Spēlētāja ID*/
    int gameID;
    char* name;/* Spēlētāja vārds */
    coordinate_t position; /* Spēlētāja koordinātas */
    float angle; /* Spēlētāja leņķis */
    float speed; /*Spēlētāja ātrums*/
    float acceleration; /*Spēlētāja paātrinājums*/
    int laps; /* Cik reizes spēlētājs jau apbraucis apkārt kartei */
    struct Player_info* next;
    struct Player_info* prev;
}player_t;

typedef struct Player_list{
    int count;
    player_t* head;
    player_t* tail;
}playerlist_t;

void playerlistPush(playerlist_t** list, player_t** player){
        if((*list)->head == NULL){
        (*list)->head = *player;
        (*list)->tail = *player;
        (*list)->count++;
    }
    else{
        (*list)->tail->next = *player;
        (*player)->prev = (*list)->tail;
        (*list)->tail = *player;
        (*list)->count++;
    }
    return;
}
void deletePlayer(playerlist_t** list, int playerId){
    player_t* current = (*list)->tail;
    while(current != NULL){
        if(current->ID == playerId){
            if(current == (*list)->head){
                (*list)->head = (*list)->head->next;
                (*list)->head->prev = NULL;
                free(current);
            }
            else if(current == (*list)->tail){
                (*list)->tail = (*list)->tail->prev;
                (*list)->tail->next = NULL;
                free(current);
            }
            else{
                current->prev->next = current->next;
                current->next->prev = current->prev;
                free(current);
            }
            return;    
        }
        current = current->next;
    }
    return;
}

// void serverplayerlistPush(playerlist_t** list){
//     player_t* player = (player_t*)malloc(sizeof(player_t));
//     player->angle = P_START_ANGLE;
//     player->position.x = P_START_X;
//     player->speed = 0;
//     player->acceleration = 0;
//     player->laps = 0;
//     player->next = NULL;
//     player->prev = NULL;
    
//     if((*list)->head == NULL){
//         player->ID = 1;
//         player->position.y = P_START_Y;
//         (*list)->head = player;
//         (*list)->tail = player;
//         (*list)->count++;
//         return;
//     }
//     else{
//         player->ID = (*list)->tail->ID + 1;
//         player->position.y = P_START_Y + (*list)->count*2;
//         (*list)->tail->next = player;
//         player->prev = (*list)->tail;
//         (*list)->tail = player;
//         (*list)->count++;
//     }
//     //:TODO SEND PLAYER ID TO PLAYER
//     return;
// }
#endif