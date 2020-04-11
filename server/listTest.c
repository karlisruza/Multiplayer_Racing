#include <stdlib.h>
#include <stdio.h>
#include "../car/player.h"
#include "../car/car.h"

int main(void){
    struct playerList* list= malloc (sizeof(struct playerList));
    int fd = 1;

    pushPlayer(&list, 1);
    pushPlayer(&list, 2);
    pushPlayer(&list, 3);
    pushPlayer(&list, 4);
    pushPlayer(&list, 5);

    struct car* player1 = malloc(sizeof(struct car));
    initCar(&player1);

    player1->angle=0;
	player1->head->x = 75;
	player1->head->y = 31;
	player1->mid->x = 74;
	player1->mid->y = 31;
	player1->tail->x = 73;
	player1->tail->y = 31;

    setCarData(1, player1, &list);
    printClientList(list);
    return 1;
}