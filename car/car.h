#include <stdlib.h>

#ifndef _CAR_HEADER
#define _CAR_HEADER
struct coords{
	int x;
	int y;
};
struct car{
	int playerId;
	struct coords* head;
	struct coords* mid;
	struct coords* tail;
	int angle;
};
void initCar(struct car** player){
	(*player)->head = malloc(sizeof(struct coords));
	(*player)->mid = malloc(sizeof(struct coords));
	(*player)->tail = malloc(sizeof(struct coords));
	return;
}
void printCar(struct car* player){
	printf("playerId: %d\nhx: %d\nhy: %d\nmx: %d\nmy: %d\ntx: %d\nty: %d\nangle: %d\n", 
	player->playerId, player->head->x, player->head->y, player->mid->x, player->mid->y,
	player->tail->x, player->tail->y, player->angle);
}
#endif