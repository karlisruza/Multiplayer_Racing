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
#include "connect/connect.h"
#include "handleData/handleData.h"
#include "sendMessage/sendName.h"
#include "sendMessage/requestGame.h"
#include "sendMessage/createGame.h"
#include "sendMessage/joinGame.h"
#include "sendMessage/requestPlayer.h"
#include "sendMessage/ping.h"

#include "display/const.h"
#include "display/car.h"
#include "display/graphics.h"
#include "display/controls.h"

struct playerListener {    
    pthread_t threadID;        // ID returned by pthread_create()
    int       charCount;       // printCount passed through Terminal
};

void *joinEvent(void *threadStructPointer){ 

    //makes threadInfo struct available
    struct playerListener *tinfo = threadStructPointer;

    printf("aaaaaaaaaaaaaaaa");

    //most likely an infinte while loop that, upon recieving a note from the server, 
        //updates the player list. 

    //if possible, check like once a second or more rarely

    //once host forces game launch, recieves notif, updates the local playerList, and 
        //allows game launch. 

    //frees all resources the thread had taken up.
    pthread_exit (NULL);
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

    WINDOW* win = startGraphics();  //graphics.h
    enableRawMode();                //controls.h

    enterName(win);                 //in graphics.h
    clientPlayer->name = writePrompt(win, 3, 4); //in controls.h


    if(clientPlayer->name != NULL){
        clientPlayer->ID = sendName(clientPlayer->name, clientFd); //receives id as response from server
    } 
    else{
        exit(1);
    }
    
    //populate gamelist and then display it, and initialize navigator
    requestGame(&gameList, clientFd);
    displayGameList(win, &gameList); //in graphics.h

    //create game function called in gameListNav, joinGame if returns 0;
    if(gameListNav(win, &gameList, &clientPlayer, clientFd) == 0){
        joinGame(&playerList, &clientPlayer, clientFd);
    }
    else{
        endwin();
        exit(1);
    }
    // endwin();
    requestPlayer(&playerList, &clientPlayer, clientFd);

    drawLobby(win, &playerList, clientPlayer);

    while(true){

    }

    //draw lobby




  
}
