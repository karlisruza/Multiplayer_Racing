#ifndef REQUESTUPDATEPLAYER_H_INCLUDED
#define REQUESTUPDATEPLAYER_H_INCLUDED
//return -1 error, 1 success

int requestUpdatePlayer(playerlist_t** list, player_t* clientPlayer, int clientFd, float x, float y){
    msg_t request;
    playerlist_t* playerList = *list;
    request.type = UPDATE_PLAYER;
    up_pt move;
    move.playerID = clientPlayer->ID;
    move.gameID = clientPlayer->gameID;
    move.action.x = x;
    move.action.y = y;
    memcpy((void*)&request.payload, (void*)&move, sizeof(move));
    int length = ((void*)&request.payload - (void*)&request.type) + sizeof(move);
    sendData(clientFd, (void*)&request, length, NULL);
    return 0;
}
#endif // !REQUESTGAME_H_INCLUDED
