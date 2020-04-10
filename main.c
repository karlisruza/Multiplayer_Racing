#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>
#include <math.h>
#include "car/car.h"

const int height = 40; //y axis
const int width = 150; //x axis

// struct coords{
// 	int x;
// 	int y;
// };
// struct car{
// 	int playerId;
// 	struct coords* head;
// 	struct coords* mid;
// 	struct coords* tail;
// 	int angle;
// };
// void initCar(struct car** player){
// 	(*player)->head = malloc(sizeof(struct coords));
// 	(*player)->mid = malloc(sizeof(struct coords));
// 	(*player)->tail = malloc(sizeof(struct coords));
// 	return;
// }
void moveCar(struct car** player, WINDOW* win, bool forward){
	int hx = (*player)->head->x;
	int hy = (*player)->head->y;
	int tx = (*player)->tail->x;
	int ty = (*player)->tail->y;

    switch((*player)->angle){
	  	case 0: //0
	  		if(forward){
	  			hx += 1;
	  		}
	  		else{
	  			tx -= 1;
	  		}
      		break;
      	case 1: // 315*
	  		if(forward){
	  			hx += 1;
	  			hy += 1;
	  		}
	  		else{
	  			tx -= 1;
	  			ty -= 1;
	  		}
      		break;
      	case 2: // 270*
	  		if(forward){
	  			hy += 1;
	  		}
	  		else{
	  			ty -= 1;
	  		}
   			break;
   		case 3: // 225*
	  		if(forward){
	  			hx -= 1;
	  			hy += 1;
	  		}
	  		else{
	  			tx += 1;
	  			ty -= 1;	
	  		}
      		break;
      	case 4:	//180*
	  		if(forward){
	  			hx -= 1;
	  		}
	  		else{
	  			tx += 1;
	  		}
   			break;
   		case 5:	//135*
	  		if(forward){
	  			hx -= 1;
	  			hy -= 1;
	  		}
	  		else{
	  			tx += 1;
	  			ty += 1;
	  		}
      		break;
      	case 6: //90*
	  		if(forward){
	  			hy -= 1;
	  		}
	  		else{
	  			ty += 1;
	  		}
   			break;
	    case 7: //45*
	  		if(forward){
	  			hx += 1;
	  			hy -= 1;
	  		}
	  		else{
	  			tx -= 1;
	  			ty += 1;
	  		}
   			break;
	}
	if(forward){
		if(mvwinch(win, hy, hx) == ' '){
			mvwprintw(win, (*player)->tail->y, (*player)->tail->x, " ");
			(*player)->tail->x = (*player)->mid->x;
			(*player)->tail->y = (*player)->mid->y;
			(*player)->mid->x = (*player)->head->x;
			(*player)->mid->y = (*player)->head->y;
			(*player)->head->x = hx;
			(*player)->head->y = hy;
		}
		return;
	}
	else{
		if(mvwinch(win, ty, tx) == ' '){
			mvwprintw(win, (*player)->head->y, (*player)->head->x, " ");
			(*player)->head->x = (*player)->mid->x;
			(*player)->head->y = (*player)->mid->y;
			(*player)->mid->x = (*player)->tail->x;
			(*player)->mid->y = (*player)->tail->y;
			(*player)->tail->x = tx;
			(*player)->tail->y = ty;
		}
		return;		
	}
}
void rotateCar(struct car** player, WINDOW* win, bool clockwise){
	if(clockwise){
		(*player)->angle +=1;
		if((*player)->angle == 8){
	   		(*player)->angle = 0;
	    }
	}
	else{
		(*player)->angle -=1;
		if((*player)->angle == -1){
	   		(*player)->angle = 7;
	    }
	}
	mvwprintw(win, 2, 2, "%d", (*player)->angle);

	int hx = (*player)->mid->x;
	int hy = (*player)->mid->y;
	int tx = (*player)->mid->x;
	int ty = (*player)->mid->y;

	//rotates around the middle of the car (adjusts coordinates of each car piece)
    switch((*player)->angle){
	  	case 0: //0 *
      		hx = (*player)->mid->x + 1;
      		tx = (*player)->mid->x - 1;
      		break;
      	case 1: // 315*
      		hx = (*player)->mid->x + 1;
      		hy = (*player)->mid->y + 1;
      		tx = (*player)->mid->x - 1;
      		ty = (*player)->mid->y - 1;
      		break;
      	case 2: // 270*
      		hy = (*player)->mid->y + 1;
      		ty = (*player)->mid->y - 1;
   			break;
   		case 3: // 225*
      		hx = (*player)->mid->x - 1;
      		hy = (*player)->mid->y + 1;
      		tx = (*player)->mid->x + 1;
      		ty = (*player)->mid->y - 1;
      		break;
      	case 4:	//180*
      		hx = (*player)->mid->x - 1;
      		tx = (*player)->mid->x + 1;
   			break;
   		case 5:	//135*
   		    hx = (*player)->mid->x - 1;
      		hy = (*player)->mid->y - 1;
      		tx = (*player)->mid->x + 1;
      		ty = (*player)->mid->y + 1;
      		break;
      	case 6: //90*
      	    hy = (*player)->mid->y - 1;
      		ty = (*player)->mid->y + 1;
   			break;
	    case 7: //45*
	        hx = (*player)->mid->x + 1;
      		hy = (*player)->mid->y - 1;
      		tx = (*player)->mid->x - 1;
      		ty = (*player)->mid->y + 1;
   			break;
	}

	//checks whether movement is blocked or not, if not, changes car coordinates
	if(mvwinch(win, hy, hx) == ' '){
		if(mvwinch(win, ty, tx) == ' '){
			mvwprintw(win, (*player)->head->y, (*player)->head->x, " ");
			mvwprintw(win, (*player)->tail->y, (*player)->tail->x, " ");
			(*player)->head->y = hy;
			(*player)->head->x = hx;
			(*player)->tail->y = ty;
			(*player)->tail->x = tx;
			return;
		}
	}

	//if previous if fails, changes angle back to previous, because rotation was not possible
	if(clockwise){
		(*player)->angle -=1;
		if((*player)->angle == -1){
	   		(*player)->angle = 7;
	    }
	}
	else{
		(*player)->angle +=1;
		if((*player)->angle == 8){
	   		(*player)->angle = 0;
	    }
	}
	return;
}
	//1 = 0*; 2 = 45*; 3 = 90*; 4 = 135*; 5 = 180*; 6 = 225*; 7 = 270*; 8 = 315*
	// ==0		  0			0	    0		0==		     =		    =        =		
	//			 =			=	     =                  =		    =         =
	//			=			=         =                0		    0          0

void keyPress(struct car** player, WINDOW* win){
	int key = getch();
	switch (key)
	{
	    case 'w':
			moveCar(&(*player), win, true);
	     	break;
	    case 's':
	    	moveCar(&(*player), win, false);
	    	break;

	    case 'a':
	    	rotateCar(&(*player), win, false);
	     	break;
	    case 'd':
	    	rotateCar(&(*player), win, true);
	     	break;
	}
}


void drawCar(WINDOW* win, struct car** player){
		mvwprintw(win, (*player)->head->y, (*player)->head->x, "0");
		mvwprintw(win, (*player)->mid->y, (*player)->mid->x, "=");
		mvwprintw(win, (*player)->tail->y, (*player)->tail->x, "=");
}

void drawMap(WINDOW * win){
	double realx, realy;
	for(int y = 1; y < height-1;y++){
		for(int x = 1; x < width-1; x++){
			realx = x - width/2;
			realy = (y - height/2)*3;

			if((realx)*(realx) + (realy)*(realy) > (width/2) * (width/2)){
				mvwprintw(win, y, x, "x");
			}
		}
	}
	for(int y = 10; y < height-10;y++){
		for(int x = 5; x < width- 8; x++){
			realx = x - width/2;
			realy = (y - height/2)*3;

			if((realx)*(realx) + (realy)*(realy) < ((height)/2) * (height/2) * 6){
				mvwprintw(win, y, x, "x");
			}
		}
	}
	return;
}
void printCoords(struct car** player, WINDOW* win){
	mvwprintw(win, 6, 6, "head x: %d, head y: %d\n mid x: %d, mid y: %d\n tail x: %d tail y: %d\n",
		(*player)->head->x, (*player)->head->y, (*player)->mid->x, (*player)->mid->y, (*player)->tail->x, (*player)->tail->y
	);
	return;
}

int main(void){
	initscr();
	noecho();
	
	WINDOW * win = newwin(height, width, 0, 0);
	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
	refresh();

	drawMap(win);
	wrefresh(win);

	struct car* player1 = malloc(sizeof(struct car));
	initCar(&player1);
	player1->angle=0;
	player1->head->x = 75;
	player1->head->y = 31;
	player1->mid->x = 74;
	player1->mid->y = 31;
	player1->tail->x = 73;
	player1->tail->y = 31;

	drawCar(win, &player1);
	wrefresh(win);
	refresh();

	while(true){
		keyPress(&player1, win);
		// printCoords(&player1, win);
		drawCar(win, &player1);
		wrefresh(win);
		refresh();
	}

	endwin();


	return 0;
}