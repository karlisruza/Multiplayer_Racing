#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h> 
#include "../car/car.h"
#include "../protocol/message.h"
#include "../protocol/payload.h"
#include <ncurses.h>

void keyPress(int playerid, int gameid, int fd){
	int key = getch();
    msg_t message;
    message.type = UP;

    up_pt data;
    data.playerID = playerid;
    data.gameID = gameid;

    action_t keypress; //(0, 1) UP (0, -1) Down (1, 0) Right (-1, 0) Left
	switch (key)
	{
	    case 'w':
            keypress.x = 0;
            keypress.y = 1;
	     	break;
	    case 's':
            keypress.x = 0;
            keypress.y = -1;
	    	break;
	    case 'a':
            keypress.x = -1;
            keypress.y = 0;
	     	break;
	    case 'd':
            keypress.x = 1;
            keypress.y = 0;
	     	break;
	}
    data.action = keypress;
    memcpy((void*)&message.payload, (void*)&data, sizeof(data));
    int length = ((void*)&message.payload - (void*)&message.type) + sizeof(data);
    sendData(fd, (void*)&message, length, NULL);
}

int main(int argc, char* argv[]){
    char* ptr;
    int cPort = strtol(argv[1], &ptr, 10);
    int sPort = strtol(argv[3], &ptr, 10);
    if(!cPort || !sPort){
        perror("invalid args");;
    }

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("tcp socket error");
        return -1;
    }
    //set protocol to ipv4 and set port
    struct sockaddr_in myAddr;
    memset(&myAddr, 0, sizeof(struct sockaddr_in));
    myAddr.sin_family = AF_INET; //set family to ipv4
    myAddr.sin_port = htons(cPort); //htons converts to network byte order

    //bind address to socket
    if (bind(clientSocket, (struct sockaddr *) &myAddr, sizeof(struct sockaddr_in)) == -1) {
        perror("tcp bind");
        return -1;
    }

    //server address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(sPort);
    inet_aton(argv[2], &serverAddr.sin_addr);
    //connect to server address
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));

    player_t player;
    player.ID = 1;
    

    msg_t message;
    message.type = PING;

    int length = ((void*)&message.payload - (void*)&message.type);
    msg_t* msgr;
    int i = 0;
    while(true){
        keyPress(player.ID, player.gameID, clientSocket);
        // void* buffer = malloc(sizeof(MAX_PAYLOAD_SIZE+sizeof(msg_t)));
        // send(clientSocket, (void*)&message, length, 0);
        // recv(clientSocket, buffer, sizeof(msg_t), 0);
        // msgr = (msg_t*)buffer;
        // printf("b4 handle %d \n", msgr->type);
        // handleData(msgr, clientSocket);

        // free(buffer);
        // i++;
    }
}