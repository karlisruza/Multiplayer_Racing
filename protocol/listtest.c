#include <stdio.h>
#include <stdlib.h>
#include "./protocol.h"
#include "./racinglist.h"

int main(void){
    gamelist_t* gamelist = malloc(sizeof(gamelist_t));
    playerlist_t* playerlist = malloc(sizeof(playerlist_t));

    // playerlistPush(&playerlist);
    // playerlistPush(&playerlist);
    // playerlistPush(&playerlist);
    // playerlistPush(&playerlist);
    // playerlistPush(&playerlist);
    // printf("list count: %d\n", playerlist->count);
    // printPlayerlist(&playerlist);
    // printf("3rd: %d\n", playerlist->head->next->next->ID);
    gamelistPush(&gamelist);
    gamelistPush(&gamelist);
    gamelistPush(&gamelist);
    gamelistPush(&gamelist);
    gamelistPush(&gamelist);

    playerlistPush(&gamelist->head->playerlist);
    playerlistPush(&gamelist->head->playerlist);
    playerlistPush(&gamelist->head->playerlist);
    playerlistPush(&gamelist->head->playerlist);
    playerlistPush(&gamelist->head->playerlist);

    printGamelist(&gamelist);
    printPlayerlist(&(gamelist->head->playerlist));

}
