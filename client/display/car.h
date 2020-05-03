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
	int midMark;
};

 	//obsolete?
void initCar(struct car** player){
	(*player)->mid = malloc(sizeof(struct coords));
	return;
}

	//creates the car object on the finish line
struct car* initPlayer (){
	struct car* player = malloc(sizeof(struct car));
	initCar(&player);
	player->angle=0;
	player->velocity=0;
	player->mid->x = 74.0;
	player->mid->y = 31.0;
	return player;
};

void printCar(struct car* player){
	printf("playerId: %d\nmx: %f\nmy: %f\nv: %f\nangle: %f\n", 
	player->playerId, player->mid->x, player->mid->y,
	player->velocity, player->angle);
}



	//since fields due to colour in curses are not simply chars anymore,
		//this function checks if the object in front is a wall. 
		//blanks, finish lines, decorations, and other cars are transparent. 
bool checkMove (WINDOW * win, double y, double x){
	int intx = round(x);
	int inty = round(y);


	if ((mvwinch(win, inty, intx) & A_CHARTEXT) != 'x'){
		return true;
	}
	
	else{
		return false;
	}
}



		//gradually moves car forward, removing some speed each iteration.
void intertia(struct car** player, WINDOW* win){

	if (((*player) -> velocity) > 0.01){
		(*player) -> velocity -= FRICTION;
	} else if (((*player) -> velocity) < -0.01){
		(*player) -> velocity += FRICTION;
	} else {
		return;
	}

	double newXHead = ((*player)->mid->x) 
		+ (*player)->velocity * cos((*player)->angle * M_PI) 
		+ cos((*player)->angle * M_PI);

	double newYHead = ((*player)->mid->y) 
		+ (*player)->velocity * sin((*player)->angle * M_PI) 
		+ sin((*player)->angle * M_PI);

	double newXTail = ((*player)->mid->x) 
		+ (*player)->velocity * cos((*player)->angle * M_PI) 
		- cos((*player)->angle * M_PI);

	double newYTail = ((*player)->mid->y) 
		+ (*player)->velocity * sin((*player)->angle * M_PI) 
		- sin((*player)->angle * M_PI);

	if(checkMove(win, newYHead, newXHead) && checkMove(win, newYTail, newXTail)){
		//mvwprintw(win, (*player)->tail->y, (*player)->tail->x, " ");
		(*player)->mid->x = newXHead - 1*cos((*player)->angle * M_PI);
		(*player)->mid->y = newYHead - 1*sin((*player)->angle * M_PI);
	}
	else 
		(*player)->velocity = 0;

		//checks if the car has reached more than 50% of the track so you cannot win by driving on the finish line
		//the finish line is defined at x = 74; firstly checks if the player is in the upper side of the loop
	
	return;
}


	//provides forward motion when the forward motion key is pressed
void moveCar(struct car** player, WINDOW* win, bool forward){

	if(forward){
		(*player) -> velocity += ACCELERATION;
	}
	else {
		(*player) -> velocity -= ACCELERATION;
	}

	if ((*player) -> velocity >= MAX_SPEED){
		(*player) -> velocity = MAX_SPEED;
	} else if ((*player) -> velocity <= MAX_REV_SPEED){
		(*player) -> velocity = MAX_REV_SPEED;
	}


	double newXHead = ((*player)->mid->x) 
		+ (*player)->velocity * cos((*player)->angle * M_PI) 
		+ cos((*player)->angle * M_PI);

	double newYHead = ((*player)->mid->y) 
		+ (*player)->velocity * sin((*player)->angle * M_PI) 
		+ sin((*player)->angle * M_PI);

	double newXTail = ((*player)->mid->x) 
		+ (*player)->velocity * cos((*player)->angle * M_PI) 
		- cos((*player)->angle * M_PI);

	double newYTail = ((*player)->mid->y) 
		+ (*player)->velocity * sin((*player)->angle * M_PI) 
		- sin((*player)->angle * M_PI);

	if(checkMove(win, newYHead, newXHead) && checkMove(win, newYTail, newXTail)){
		//mvwprintw(win, (*player)->tail->y, (*player)->tail->x, " ");
		(*player)->mid->x = newXHead - 1*cos((*player)->angle * M_PI);
		(*player)->mid->y = newYHead - 1*sin((*player)->angle * M_PI);
	}
	else 
		(*player)->velocity = 0;

		//checks if the car has reached more than 50% of the track so you cannot win by driving on the finish line
		//the finish line is defined at x = 74; firstly checks if the player is in the upper side of the loop
	
	return;
}

		//checks if D is pressed on the keyboard. If true, rotates the car clockwise. 
		//otherwise, it is considered A was pressed, and will turn counter-clockwise
void rotateCar(struct car** player, WINDOW* win, bool clockwise){

	float tempAngle;
	if(clockwise){
		tempAngle = -TURN_SPEED;
	}
	else{
		tempAngle = TURN_SPEED;
	}
		//temporary variables to hold the checkable values 
	double newXHead = ((*player)->mid->x) + 
		cos(((*player)->angle + tempAngle) * M_PI);

	double newYHead = ((*player)->mid->y) + 
		sin(((*player)->angle + tempAngle) * M_PI);

	double newXTail = ((*player)->mid->x) -
		cos(((*player)->angle + tempAngle) * M_PI);

	double newYTail = ((*player)->mid->y) -
		sin(((*player)->angle + tempAngle) * M_PI);


	if(checkMove(win, newYHead, newXHead) && checkMove (win, newYTail, newXTail)){
		(*player)->angle += tempAngle;
	}	else 
		(*player)->velocity = 0;

	return;
}


void checkLapCrosing(struct car** player, WINDOW* win){

			// 4 > < | 8 > > 
			//---------------
			// 1 < < | 2 < >

	if 		((*player)->mid->y < MAP_HEIGHT/2 && (*player)->mid->x < MAP_WIDTH/2)
	{
		if ((*player)->midMark % 2 < 1) (*player)->midMark += 1;
	} 
	else if ((*player)->mid->y < MAP_HEIGHT/2 && (*player)->mid->x > MAP_WIDTH/2)
	{
		if ((*player)->midMark % 4 < 2) (*player)->midMark += 2;
	} 
	else if ((*player)->mid->y > MAP_HEIGHT/2 && (*player)->mid->x < MAP_WIDTH/2)
	{
		if ((*player)->midMark % 8 < 4) (*player)->midMark += 4;
	} 
	else 
	{
		if ((*player)->midMark % 16 < 8) (*player)->midMark += 8;
	}

		//if all midMarks collected, looks for finish line
	if ((*player)->midMark == 15){

		int intx = round((*player)->mid->x);
		int inty = round((*player)->mid->y);
		if ((mvwinch(win, inty, intx) & A_CHARTEXT) == '|'){
			(*player)->laps++;
			(*player)->midMark = 0;	
			return;
		}
	}
}


#endif