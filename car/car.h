#include <stdlib.h>

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