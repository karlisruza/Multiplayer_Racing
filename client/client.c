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


    // //thread for user key input;
    tparams_t* params = (tparams_t*)malloc(sizeof(tparams_t));
                //via server or when create game > join game 

    	//if 0, then is host. If 1, then simply joins a game.
    int gameListRet = gameListNav(win, &gameList, &clientPlayer, clientFd);
    if(gameListRet == 0){
    	params->isHost = true;
    } else if (gameListRet == 1){
    	params->isHost = false;
    } else {
        close(clientFd);
        die("game list navigator made an error\n");
    }
    
    if (joinGame(&playerList, &clientPlayer, clientFd) < 0){
    	die("join game failed\n");
    }
    //start game
    params->clientPlayer = clientPlayer;
    params->playerList = playerList;
    params->clientFd = clientFd;
    

    if (requestPlayer(&playerList, &clientPlayer, clientFd)<0){
    	die("requestPlayerFailed");
    }

    drawLobby(win, &playerList, clientPlayer);   
    char* buffer = (void*)malloc(sizeof(msg_t));
    int length = (sizeof(msg_t));

    pthread_t thread;
    pthread_create(&thread, NULL, lobbyInput, (void*)params);

    while(1){
        //gaida incoming msgs (start game msg vai newPlayerMsg)
        int retLen = recv(clientFd, (void*)buffer, length, 0);

        if(retLen < 0){
            printf("fail \n");
        }

        msg_t* msgr = (msg_t*)buffer;
        handleData(msgr, &playerList, clientPlayer, clientFd);

        if(msgr->type == PLAYER_JOINED){
            drawLobby(win, &playerList, clientPlayer);
        } 
        else if(msgr->type == START_GAME){ 
            break;       
        }
    }

    pthread_join(thread, NULL);
    free(params);

    wrefresh(win);

    tparams_t* paramsTwo = (tparams_t*)malloc(sizeof(tparams_t));

    paramsTwo->clientPlayer = clientPlayer;
    paramsTwo->playerList = playerList;
    paramsTwo->clientFd = clientFd;
    pthread_t threadTwo;

    if (pthread_create(&thread, NULL, carControl, (void*)paramsTwo) < 0){
        die("failed the second thread\n");
    }
    
    drawMap(win);
    wrefresh(win);
    while (thread){
        // clientPlayer->x += 2.432;
        // mvwprintw(win, 37, 37, "%f", clientPlayer->x);
        // sleep(2);
        // wrefresh(win);
        int retLen = recv(clientFd, (void*)buffer, length, 0);

        if(retLen < 0){
            printf("fail \n");
        }

        msg_t* msgr = (msg_t*)buffer;
        if(msgr->type == UPDATE_PLAYER){               
            updpos_pt* playerData;
            playerData = (updpos_pt*)msgr->payload;

            //find player and update data
            player_t* current = playerList->head;
            while(current != NULL){
                if(current->ID == playerData->ID){
                    current->x = playerData->x;
                    current->y = playerData->y;
                    current->angle = playerData->angle;
                    break;
                }
                current = current->next;
            }
            // mvwprintw(win, 37, 37, "x: %f, y:%f, a:%f ", current->x, current->y, current->angle);
            // sleep(2);
            // wrefresh(win);
            drawPlayer(win, current);
            wrefresh(win);
        }
    }

    pthread_join(thread, NULL);


    // requestGameStart(&clientPlayer, clientFd);
    //draw lobby


    die("got to end successfully.");

  
}
