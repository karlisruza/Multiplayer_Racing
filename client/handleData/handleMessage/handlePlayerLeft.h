#ifndef HANDLEPLAYERLEFT_H_INCLUDED
#define HANDLEPLAYERLEFT_H_INCLUDED

int handlePlayerLeft(msg_t* message, playerlist_t** list, int clientFd){
    playerlist_t* playerList = *list;
    cg_pt* playerData = (cg_pt*)message->payload;
    deletePlayer(&playerList, playerData->playerID);

    return 0;
}

#endif