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
#include "sendMessage/createGame.h"
#include "sendMessage/joinGame.h"
#include "sendMessage/requestPlayer.h"
#include "sendMessage/ping.h"
#include "sendMessage/requestGame.h"
#include "sendMessage/requestGameStart.h"
#include "handleData/handleData.h"
#include "sendMessage/requestGameStart.h"
#include "handleData/handleData.h"

#include "display/const.h"
#include "display/car.h"
#include "display/graphics.h"
#include "display/controls.h"


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

    atexit(fixTerminal);			//const.h
    WINDOW* win = startGraphics();  //graphics.h- launches ncurses and screen
    enableRawMode();  				//controls.h - allows reading by character not by line

    enterName(win);                 //graphics.h - shows the "enter your name" prompt.
    writePrompt(win, 3, 4, &clientPlayer); //in controls.h - displays the name you enter.

    if(clientPlayer->name != NULL){
        clientPlayer->ID = sendName(clientPlayer->name, clientFd); //receives id as response from server
    } 
    else{
        die("Couldn't properly send player name");
    }
    
    // //populate gamelist and then display it, and initialize navigator
    requestGame(&gameList, clientFd);
    displayGameList(win, &gameList); //in graphics.h

    if(gameListNav(win, &gameList, &clientPlayer, clientFd) == 0){
        joinGame(&playerList, &clientPlayer, clientFd);
    }
    else{
        close(clientFd);
        die("join game failed\n");
    }

    //start game

    // //thread for user key input;
    tparams_t* params = (tparams_t*)malloc(sizeof(tparams_t));
    params->clientPlayer = clientPlayer;
    params->clientFd = clientFd;
    params->isHost = true; //This applies to the game creator - could provide it to the player 
                //via server or when create game > join game 
    pthread_t thread;
    pthread_create(&thread, NULL, lobbyInput, (void*)params);     

    requestPlayer(&playerList, &clientPlayer, clientFd);

    drawLobby(win, &playerList, clientPlayer);   
    char* buffer = (void*)malloc(sizeof(msg_t));
    int length = (sizeof(msg_t));
    while(true){
        //gaida incoming msgs (start game msg vai newPlayerMsg)
        int retLen = recv(clientFd, (void*)buffer, length, 0);
        if(retLen < 0){
            printf("fail \n");
        }

        msg_t* msgr = (msg_t*)buffer;
        // printf("message type: %d\n", msgr->type);
        handleData(msgr, &playerList, clientPlayer, clientFd);
        if(msgr->type = PLAYER_JOINED){
            drawLobby(win, &playerList, clientPlayer);
        }
        sleep (3);
    }

    // requestGameStart(&clientPlayer, clientFd);
    //draw lobby




  
}
