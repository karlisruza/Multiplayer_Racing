#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "car.h"
#ifndef _PLAYER_HEADER
#define _PLAYER_HEADER

struct player{
    int clientSocket;
    struct car* carData;
    struct player* next;
    struct player* prev;
};

//list of players and their data to pass to threads
struct playerList{
    int playerCount;
    struct player* head;
    struct player* tail;
};

void pushPlayer(struct playerList** list, int clientFd){
    struct player* client = malloc(sizeof(struct player));
    client->clientSocket = clientFd;
    client->carData = malloc(sizeof(struct car));
    initCar(&client->carData);
    if((*list)->head == NULL){
        (*list)->head = client;
        (*list)->tail = client;
        (*list)->playerCount++;
    }
    else{
        (*list)->tail->next = client;
        client->prev = (*list)->tail;
        (*list)->tail = client;
        (*list)->playerCount++;
    }
    return;
}

void printClient(int fd) {
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);

    if (getpeername(fd, (struct sockaddr *) &addr, &addrLen) == -1) {
        perror("getpeername");
        return;
    }

    printf("%s:%d connected via TCP\n",
	    inet_ntoa(addr.sin_addr),
	    ntohs(addr.sin_port));
}

void printClientList(struct playerList* list){
    struct player* current = list->head;
    while(current != NULL){
        printf("clientfd: %d\n", current->clientSocket);
        printCar(current->carData);
        current = current->next;
    }
}
void setCarData(int clientFd, struct car* player, struct playerList** list){
    struct player* current = (*list)->head;
    while(current != NULL){
        if(clientFd == current->clientSocket){
            struct car* tempCar = current->carData;
            current->carData = player;
            free(tempCar);
            return;
        }
        current = current->next;
    }
    perror("Client not in player list"); 
}


#endif