#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ncurses.h>
#include <math.h> 
#include <pthread.h> 

#include "../data/payload/payload.h"
#include "../data/game/game.h"
#include "../data/message.h"
#include "connect/connect.h"
#include "handleData/handleData.h"
#include "sendMessage/sendName.h"
#include "sendMessage/requestGame.h"
#include "sendMessage/createGame.h"
#include "sendMessage/joinGame.h"
#include "sendMessage/requestPlayer.h"
#include "sendMessage/ping.h"
#include "sendMessage/requestGameStart.h"
#include "handleData/handleData.h"

#include "display/const.h"
#include "display/car.h"
#include "display/graphics.h"
#include "display/controls.h"

// struct playerListener {    
//     pthread_t threadID;        // ID returned by pthread_create()
//     int       charCount;       // printCount passed through Terminal
// };

typedef struct tparams{
    player_t* clientPlayer;
    int clientFd;
}tparams_t;

void *userInput(void* params){ 
    int clientFd = ((tparams_t*)params)->clientFd;
    player_t* clientPlayer = ((tparams_t*)params)->clientPlayer;
    char c = getchar();
    switch (c){
        case 's':
            requestGameStart(clientPlayer, clientFd);
            break;
        default:
            break;
    }
}

int main(int argc, char* argv[]){
    // ./c 8015 10.0.2.15 8014
    int clientFd;
    if (argc > 1){
        clientFd = clientConnect(argv[1], argv[2], argv[3]); //sets up connection
    } 
    else{    
        printf("There are three arguments - client port, server IP and server port. Autoset to 8015, 127.0.0.1, and 8014.\n");
        clientFd = clientConnect("8015", "127.0.0.1", "8014"); //sets up connection
    }

    player_t* clientPlayer = (player_t*)malloc(sizeof(player_t));
    playerlist_t* playerList = (playerlist_t*)malloc(sizeof(playerlist_t)); //contains all players in lobby
    gamelist_t* gameList = (gamelist_t*)malloc(sizeof(gamelist_t));

    // WINDOW* win = startGraphics();  //graphics.h
    // enableRawMode();                //controls.h

    // enterName(win);                 //in graphics.h
    // writePrompt(win, 3, 4, &clientPlayer); //in controls.h
    
    // strcpy(clientPlayer->name, "karlisxx");
    strcpy(clientPlayer->name, "karlis");
    if(clientPlayer->name != NULL){
        clientPlayer->ID = sendName(clientPlayer->name, clientFd); //receives id as response from server
    } 
    else{
        exit(1);
    }
    
    // //populate gamelist and then display it, and initialize navigator
    requestGame(&gameList, clientFd);
    // displayGameList(win, &gameList); //in graphics.h
    printf("---------------------------------game--list-------------------------\n");
    printGameList(&gameList);

    if(argc > 4){
        if(createGame(&clientPlayer, clientFd) != 0){
            perror("createGame failed\n");
            close(clientFd);
            return -1;
        }
    }
    else{
        clientPlayer->gameID = 1;
    }
    fflush(stdout);
    printf("..joined game\n");
    joinGame(&playerList, &clientPlayer, clientFd);

    requestPlayer(&playerList, &clientPlayer, clientFd);
    printPlayerList(&playerList);

    //start game
    // requestGameStart(&clientPlayer, clientFd);

    char* buffer = (void*)malloc(sizeof(msg_t));
    int length = (sizeof(msg_t));

    // //thread for user key input;
    // tparams_t* params = (tparams_t*)malloc(sizeof(tparams_t));
    // params->clientPlayer = clientPlayer;
    // params->clientFd = clientFd;
    // pthread_t thread;
    // pthread_create(&thread, NULL, userInput, (void*)params);

    while(true){
        int retLen = recv(clientFd, (void*)buffer, length, 0);
        if(retLen < 0){
            printf("fail \n");
        }
        fflush(stdout);
        msg_t* msgr = (msg_t*)buffer;
        printf("message type: %d\n", msgr->type);
        handleData(msgr, &playerList, clientFd);
        printf("---------updated list----------\n");
        printPlayerList(&playerList);
    }
    
    // close(clientFd);

    // //create game function called in gameListNav, joinGame if returns 0;
    // if(gameListNav(win, &gameList, &clientPlayer, clientFd) == 0){
    //     joinGame(&playerList, &clientPlayer, clientFd);
    // }
    // else{
    //     endwin();
    //     exit(1);
    // }
    // // // endwin();
    // requestPlayer(&playerList, &clientPlayer, clientFd);
    // drawLobby(win, &playerList, clientPlayer);

    while(true){

    }

    //draw lobby




  
}
