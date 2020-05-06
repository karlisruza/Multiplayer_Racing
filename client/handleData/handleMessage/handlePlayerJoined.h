#ifndef HANDLEPLAYERJOINED_H_INCLUDED
#define HANDLEPLAYERJOINED_H_INCLUDED

int handlePlayerJoined(msg_t* message, playerlist_t** list, int clientFd){
    playerlist_t* playerList = *list;
    rl_pt* newPlayer = (rl_pt*)message->payload;
    player_t* current = playerList->head;
    
    // //checks if player already in list
    // while(current != NULL){
    //     if(current->ID = newPlayer->ID){
    //         return -1;
    //         perror("hpj: player not in list\n");
    //     }
    //     current = current->next;
    // }
    //create new player and add to playerList
    player_t* player = (player_t*)malloc(sizeof(player_t));
    player->ID = newPlayer->ID;
    printf("newplayerid: %d\n", player->ID);
    strcpy(player->name, newPlayer->name);
    playerlistPush(&playerList, &player);

    return 0;
}

#endif