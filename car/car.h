#include <stdlib.h>

#ifndef _CAR_HEADER
#define _CAR_HEADER
struct coords{
	double x;
	double y;
};

struct car{
	int playerId;
	struct coords* mid;
	double velocity;
	double angle;
	int laps;
	bool midMark;
};

void initCar(struct car** player){
	(*player)->mid = malloc(sizeof(struct coords));
	return;
}

void printCar(struct car* player){
	printf("playerId: %d\nmx: %f\nmy: %f\nv: %f\nangle: %f\n", 
	player->playerId, player->mid->x, player->mid->y,
	player->velocity, player->angle);
}
#endif