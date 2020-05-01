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
#include "../data/payload/payload.h"
#include "../data/game/game.h"
#include "graphics/graphics.h"
#include "connect/connect.h"
#include "handleData/handleData.h"
#include "sendMessage/sendName.h"
#include "sendMessage/requestGame.h"
#include "sendMessage/createGame.h"
#include "sendMessage/joinGame.h"

int main(int argc, char* argv[]){
    int clientFd = clientConnect(argv[1], argv[2], argv[3]); //sets up connection
    // WINDOW* win = winSetup();
    player_t* clientPlayer = (player_t*)malloc(sizeof(player_t));
    playerlist_t* playerList = (playerlist_t*)malloc(sizeof(playerlist_t)); //contains all players in lobby
    gamelist_t* gameList = (gamelist_t*)malloc(sizeof(gamelist_t));

    // clientPlayer->name = enterNameMenu(win);
    clientPlayer->name = "karlisxx";
    clientPlayer->ID = sendName(clientPlayer->name, clientFd); //receives id as response

    requestGame(&gameList, clientFd);
    //displayGames()
    createGame(&clientPlayer, clientFd);
    int gameId = 4;
    joinGame(&playerList, &clientPlayer, clientFd, gameId);


    // while(true){
    //     //WaitForInput(&playerlist)-> either joingGame() or createGame()
    //     //displayLobby(&playerlist)
    //     //
    //     //waitForStart(); 
    // }



    // handleData(buffer, clientSocket, NULL, &clientplayer);
     
}