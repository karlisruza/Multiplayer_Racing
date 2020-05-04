#ifndef HANDLEREQUESTGAME_H_INCLUDED
#define HANDLEREQUESTGAME_H_INCLUDED

void handleRequestGame(gamelist_t** list, int clientFd){
    msg_t reply;
    reply.type = REQUEST_GAME;
    int length;
    gamelist_t* gameList = *list;


    if(gameList->head == NULL){
        rg_pt game;
        game.gamecount = gameList->count;
        memcpy((void*)&reply.payload, (void*)&game, sizeof(game));
        length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(game);
        sendData(clientFd, (void*)&reply, length, NULL);
    }
    else{
        game_t* current = gameList->head;
        rg_pt game;
        while(current != NULL){
            game.gameId = current->gameid;
            // game.hostId = current->playerlist->head->ID;
            game.gamecount = gameList->count;
            game.hostId = current->hostId;
            game.status = current->status;

            memcpy((void*)&reply.payload, (void*)&game, sizeof(game));
            length = ((void*)&reply.payload - (void*)&reply.type) + sizeof(game);
            sendData(clientFd, (void*)&reply, length, NULL);
            current = current->next;
        }
    }
    return;
}

#endif // !HANDLEREQUESTGAME_H_INCLUDED
