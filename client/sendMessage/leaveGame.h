#ifndef LEAVEGAME_H_INCLUDED
#define LEAVEGAME_H_INCLUDED

int leaveGame(player_t* clientPlayer, int clientFd){
    cg_pt payload;
    msg_t msg;
    msg.type = PLAYER_LEFT
    payload->playerID = clientPlayer->ID;
    payload->gameID = clientPlayer->gameID
    memcpy((void*)&msg.payload, (void*)&payload, sizeof(payload)); 
    int length = ((void*)&msg.payload - (void*)&msg.type) + sizeof(payload);
    sendData(clientFd, (void*)&msg, length, NULL);

    return 0;
}

#endif