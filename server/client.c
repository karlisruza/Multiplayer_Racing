#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h> 
#include "../car/car.h"

void carToJSON(struct car* player, char** buffer){
    //{
    //  "playerID":"1",
    //  "hx":"111",
    //  "hy":"111",
    //  "mx":
    //  "my":
    //  "tx":
    //  "ty":
    //  "angle":"1"
    //}

    // char* json = malloc(sizeof(char)*256);
    snprintf(*buffer + strlen(*buffer), sizeof(char)*256, "%d/", player->playerId);
    snprintf(*buffer + strlen(*buffer), sizeof(char)*256, "%d/", player->head->x);
    snprintf(*buffer + strlen(*buffer), sizeof(char)*256, "%d/", player->head->y);
    snprintf(*buffer + strlen(*buffer), sizeof(char)*256, "%d/", player->mid->x);
    snprintf(*buffer + strlen(*buffer), sizeof(char)*256, "%d/", player->mid->y);
    snprintf(*buffer + strlen(*buffer), sizeof(char)*256, "%d/", player->tail->x);
    snprintf(*buffer + strlen(*buffer), sizeof(char)*256, "%d/", player->tail->y);
    snprintf(*buffer + strlen(*buffer), sizeof(char)*256, "%d/", player->angle);

    // (*buffer)[2] = (char)player->head->x;
    // (*buffer)[3] = (char)player->head->y;
    // (*buffer)[4] = (char)player->mid->x;
    // (*buffer)[5] = (char)player->mid->y;
    // (*buffer)[6] = (char)player->tail->x;
    // (*buffer)[7] = (char)player->tail->y;
    // (*buffer)[8] = (char)player->angle;

    return;
}

void sendData(int socketOut, char *data, unsigned length, struct sockaddr_in *socketIn) {
    int ret;
    if (socketIn != NULL) {
	    ret = sendto(socketOut, data, length, 0, (struct sockaddr *) &socketIn, sizeof(*socket));
    } 
    if (ret < 0) {
        perror("send");
        return;
    }
}

int main(int argc, char* argv[]){
    char* ptr;
    int cPort = strtol(argv[1], &ptr, 10);
    int sPort = strtol(argv[3], &ptr, 10);

	struct car* player1 = malloc(sizeof(struct car));
	initCar(&player1);
	player1->angle=100;
	player1->head->x = 755;
	player1->head->y = 31;
	player1->mid->x = 74;
	player1->mid->y = 31;
	player1->tail->x = 73;
	player1->tail->y = 31;

    char* buffer = malloc(sizeof(char)* 256);
    carToJSON(player1, &buffer);
    printf("%s \n", buffer);
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
    char* data = "Hello, mr server! \n";
    // sendData(clientSocket, data, , &serverAddr);
    sendto(clientSocket, buffer, sizeof(char)*256, 0, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

}