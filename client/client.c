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
    params->clientPlayer = clientPlayer;
    params->clientFd = clientFd;
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


    pthread_t thread;
 	pthread_create(&thread, NULL, lobbyInput, (void*)params);
    

    if (requestPlayer(&playerList, &clientPlayer, clientFd)<0){
    	die("requestPlayerFailed");
    }


    drawLobby(win, &playerList, clientPlayer);   
    char* buffer = (void*)malloc(sizeof(msg_t));
    int length = (sizeof(msg_t));

    int steps = 0;
    while(1){
        //gaida incoming msgs (start game msg vai newPlayerMsg)
        mvwprintw(win, 30, 30, "%d - start", steps);
        wrefresh(win);
        int retLen = recv(clientFd, (void*)buffer, length, 0);

        if(retLen < 0){
            printf("fail \n");
        }
        mvwprintw(win, 31, 31, "recv - %d", steps);
        wrefresh(win);

        msg_t* msgr = (msg_t*)buffer;
        handleData(msgr, &playerList, clientPlayer, clientFd);
        mvwprintw(win, 32, 32, "handleData-dledata - %d", steps);
        wrefresh(win);

        if(msgr->type = PLAYER_JOINED){
            drawLobby(win, &playerList, clientPlayer);
            mvwprintw(win, 33, 33, "during draw lobby - %d", steps);
            wrefresh(win);

        }
		mvwprintw(win, 34, 34, "after draw lobby - %d", steps);

		wrefresh(win);

		steps++;
        sleep (1);
    }

    // requestGameStart(&clientPlayer, clientFd);
    //draw lobby


    die("got to end successfully.");

  
}
