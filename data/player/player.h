#include <stdio.h>
#include <stdlib.h>
#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

// typedef struct Coordinates{
//     float x;
//     float y;
// }coordinate_t;

typedef struct Player_info{
    int ID; /*Spēlētāja ID*/
    int gameID;
    // coordinate_t* position; /* Spēlētāja koordinātas */

    float x;
    float y;
    float angle; /* Spēlētāja leņķis */
    float speed; /*Spēlētāja ātrums*/
    float acceleration; /*Spēlētāja paātrinājums*/
    int laps; /* Cik reizes spēlētājs jau apbraucis apkārt kartei */
    struct Player_info* next;
    struct Player_info* prev;
    char name[20];/* Spēlētāja vārds */
    int midMark; /*vai ir nobraucis vairāk par pusi*/

}player_t;

typedef struct Player_list{
    int count;
    player_t* head;
    player_t* tail;
}playerlist_t;

void printPlayer(player_t* player){
    printf("    Player ID: %d\n", player->ID);
    printf("    gameID: %d\n", player->gameID);
    printf("    name: %s\n", player->name);
    printf("    position x: %f\n", player->x);
    printf("    position y: %f\n", player->y);
    printf("    angle: %f\n", player->angle);
    printf("    speed: %f\n", player->speed);
    printf("    acceleration: %f\n", player->acceleration);
    printf("    laps: %d\n", player->laps);
    printf("\n");
    return;
}

void printPlayerList(playerlist_t** list){
    player_t* current = (*list)->head;
    while(current != NULL){
        printPlayer(current);
        current = current->next;
    }
    return;
}

void playerlistPush(playerlist_t** list, player_t** player){
    if((*list)->head == NULL && (*list)->tail == NULL){
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
void removePlayer(playerlist_t** list, int playerId){
    player_t* current = (*list)->head;
    while(current != NULL){
        if(current->ID == playerId){
            if(current == (*list)->head && current == (*list)->tail){
                (*list)->head = NULL;
                (*list)->tail = NULL;
                current->next = NULL;
                current->prev = NULL;
                (*list)->count--;
            }
            else if(current == (*list)->head){
                (*list)->head = (*list)->head->next;
                (*list)->head->prev = NULL;
                current->next = NULL;
                current->prev = NULL;
                (*list)->count--;
            }
            else if(current == (*list)->tail){
                (*list)->tail = (*list)->tail->prev;
                (*list)->tail->next = NULL;
                current->next = NULL;
                current->prev = NULL;
                (*list)->count--;
            }
            else{
                current->prev->next = current->next;
                current->next->prev = current->prev;
                current->next = NULL;
                current->prev = NULL;
                (*list)->count--;
            }
            return;    
        }
        current = current->next;
    }
    return;
}

void deletePlayer(playerlist_t** list, int playerId){
    player_t* current = (*list)->tail;
    while(current != NULL){
        if(current->ID == playerId){
            if(current == (*list)->head && current == (*list)->tail){
                (*list)->head = NULL;
                (*list)->tail = NULL;
                current->next = NULL;
                current->prev = NULL;
                (*list)->count--;
                free(current);
            }
            else if(current == (*list)->head){
                (*list)->head = (*list)->head->next;
                (*list)->head->prev = NULL;
                (*list)->count--;
                free(current);
            }
            else if(current == (*list)->tail){
                (*list)->tail = (*list)->tail->prev;
                (*list)->tail->next = NULL;
                (*list)->count--;
                free(current);
            }
            else{
                current->prev->next = current->next;
                current->next->prev = current->prev;
                (*list)->count--;
                free(current);
            }
            return;    
        }
        current = current->next;
    }
    return;
}

char* getPlayerName(playerlist_t** list, int clientFd){
    playerlist_t* playerList = *list;
    player_t* current;
    if(playerList != NULL){
        if(playerList->head != NULL){
            while(current != NULL){
                if(current->ID == clientFd){
                    return current->name;
                }
            }
        }
    }
    return "Yikes";
}

void deletePlayerList(playerlist_t** list){
    playerlist_t* playerList = *list;
    player_t* temp1;
    player_t* temp2;
    if(playerList->head == playerList->tail){
        free(playerList->head);
        playerList->head = NULL;
        playerList->tail = NULL;
        playerList->count = 0;
        return;
    }
    else{
        temp1 = playerList->head;
        temp2 = playerList->head->next;
        while(temp1 != NULL){
            if(temp1 == playerList->tail){
                free(temp1);
                playerList->head = NULL;
                playerList->tail = NULL;
                playerList->count = 0;
                return;
            }
            free(temp1);
            temp1 = temp2;
            temp2 = temp2->next;
        }
    }
    return;
}


#endif