#ifndef HANDLEUPDATEPLAYER_H_INCLUDED
#define HANDLEUPDATEPLAYER_H_INCLUDED

int handleUpdatePlayer(msg_t* message, playerlist_t** list){
    playerlist_t* playerList = *list;
    updpos_pt* playerData;
    playerData = (updpos_pt*)message->payload;

    //find player and update data
    player_t* current = playerList->head;
    while(current != NULL){
        if(current->ID == playerData->ID){
            current->x = playerData->x;
            current->y = playerData->y;
            current->angle = playerData->angle;
        }
        current = current->next;
    }
    return 0;
}

#endif