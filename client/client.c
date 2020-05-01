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

int main(int argc, char* argv[]){
    int clientFd = clientConnect(argv[1], argv[2], argv[3]); //sets up connection
    // WINDOW* win = winSetup();
    player_t* clientPlayer = (player_t*)malloc(sizeof(player_t));
    gamelist_t* gameList = (gamelist_t*)malloc(sizeof(gamelist_t));

    // clientPlayer->name = enterNameMenu(win);
    clientPlayer->name = "karlisxx";
    clientPlayer->ID = sendName(clientPlayer->name, clientFd); //receives id as response

    requestGame(&gameList, clientFd);
    printGameList(&gameList);
    //displayGames()

    // handleData(buffer, clientSocket, NULL, &clientplayer);
     
}