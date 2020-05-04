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

#include "display/car.h"
#include "display/graphics.h"
#include "display/controls.h"


typedef struct Game{
    int gameid;
    int status; //(0- WAITING_PLAYERS, 1- STARTED, -1- ENDED);
    int hostId;
    playerlist_t* playerList;
    struct Game* next;
    struct Game* prev;
}game_t;

typedef struct Game_list{
    int count;
    game_t* head;
    game_t* tail;
}gamelist_t;

void displayGameCreator (WINDOW* win, gamelist_t** list){
    clear(win);
    attron(A_BOLD);
    mvwprintw(win, 1, 3, "ENTER GAME CREDENTIALS");

    game_t* newGame = (game_t*) malloc(sizeof(game_t));

    newGame

}

int displayGameList (WINDOW * win, gamelist_t** list){
    game_t* newGame = (game_t*) malloc(sizeof(game_t));

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

    WINDOW* win = startGraphics();
    enableRawMode();

    enterName(win);
    clientPlayer->name = writePrompt(win, 3, 4);

    refresh();
    wrefresh(win);


    if(clientPlayer->name != NULL){
        clientPlayer->ID = sendName(clientPlayer->name, clientFd); //receives id as response from server
    } else {
        exit(1);
    };

        //game_t current iekš graphics.h
        //if game list != null
            //if game list head != null
            // while current != null 
                //displayGameList() graphics.h
            //refresh 
            // later getch (r) -> request game no jauna
            // press number atbilstoši spelei, kurai pievienoties
            // c izsauc create game funkciju
            // join game create game rezultāts ir tāds pats
                //display lobby


    refresh();
    wrefresh(win);
    
    requestGame(&gameList, clientFd);
    displayGameList(win, &gameList); //in graphics.h
    unsigned int gameChoice = lobbyNav(win, gameList->count);

    if (gameChoice == gameList->count + 1){
        createGame

    } else {
        //join game
    }


    exit(1);


    //displayGames();
   // createGame(&clientPlayer, clientFd);
   // int gameId = 4;
   // joinGame(&playerList, &clientPlayer, clientFd, gameId);

    // while(true){
    //     //WaitForInput(&playerlist)-> either joingGame() or createGame()
    //     //displayLobby(&playerlist)
    //     //
    //     //waitForStart(); 
    // }     
}
