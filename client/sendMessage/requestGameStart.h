#ifndef REQUESTGAMESTART_H_INCLUDED
#define REQUESTGAMESTART_H_INCLUDED

int requestGameStart(player_t* clientPlayer, int clientFd){
    msg_t message;
    message.type = START_GAME;
    
    cg_pt gameinfo;
    gameinfo.gameID = clientPlayer->gameID;
    gameinfo.playerID = clientPlayer->ID;

    memcpy((void*)&message.payload, (void*)&gameinfo, sizeof(gameinfo));
    int length = ((void*)&message.payload - (void*)&message.type) + sizeof(gameinfo);
    sendData(clientFd, (void*)&message, length, NULL);

    return 0;
}

#endif