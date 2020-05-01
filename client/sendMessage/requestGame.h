#ifndef REQUESTGAME_H_INCLUDED
#define REQUESTGAME_H_INCLUDED

//return -1 error, 1 success
int requestGame(gamelist_t** list, int clientFd){
    msg_t request;
    request.type = REQUEST_GAME;
    int length = ((void*)&request.payload - (void*)&request.type);
    sendData(clientFd, (void*)&request, length, NULL);
    gamelist_t* gameList = *list;
    void* buffer = (void*)malloc(MAX_PAYLOAD_SIZE + sizeof(msg_t));
    rg_pt* gameData;

    int gamecount;
    int counter = 0;

    do{
        int ret =  recv(clientFd, buffer, sizeof(msg_t) - MAX_PAYLOAD_SIZE + sizeof(rg_pt), 0);
        printf("ret len: %d\n", ret);
        printf("iteration: %d\n", counter);
        msg_t* reply = (msg_t*)buffer;
        if(ret < 0 || reply->type != REQUEST_GAME){
            perror("REQUEST_GAME invalid response");
            return -1;
        }
        gameData = (rg_pt*)reply->payload;
        gamecount = gameData->gamecount;
        if(gamecount == 0){
            return 1;
        }
        game_t* game = (game_t*)malloc(sizeof(game_t));
        game->gameid = gameData->gameId;
        game->hostId = gameData->hostId;
        game->status = gameData->status;
        game->playerlist = NULL;        
        gamelistPush(&gameList, &game);
        counter++;
    }while(counter < gamecount);

    return 0;





            // rg_pt* gamedata = (rg_pt*)message->payload;
            // gamelist_t* list = *gamelist;
            
            // printf("gameCount: %d//////\n", gamedata->gamecount);
            // int gamecount = gamedata->gamecount;
            // char* buffer = malloc(sizeof(MAX_PAYLOAD_SIZE));
            // printf("buffersize: %ld\n", sizeof(buffer));

            // for(int i = 0; i < gamecount; i++){
            //     if(i != 0){
            //         printf("pre ret\n");
            //         int ret =  recv(clientFd, buffer, sizeof(msg_t) + MAX_PAYLOAD_SIZE, 0);
            //         printf("post ret\n");
            //         printf("buffersize: %ld\n", sizeof(buffer));

            //         message = (msg_t*)buffer;
            //     }

            //     gamedata = (rg_pt*)message->payload;
            //     game_t* game = malloc(sizeof(game_t));
            //     game->gameid = gamedata->gameId;
            //     game->hostId = gamedata->hostId;
            //     game->status = gamedata->status;
            //     game->playerlist = NULL;

            //     printGame(game);
            //     gamelistPush(&list, &game);
            //     free(message);

            //     printf("i: %d\n", i);
            // }
}

#endif // !REQUESTGAME_H_INCLUDED