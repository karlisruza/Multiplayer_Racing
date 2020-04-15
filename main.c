#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>
#include <math.h>
#include "car/car.h"

//TODO - have a press anykey to continue for the screen size thing. 
	//

#define MAP_COLOR     		COLOR_PAIR(1)
#define PLAYER_ONE_COLOR    COLOR_PAIR(2)
#define PLAYER_TWO_COLOR    COLOR_PAIR(3)

const int height = 40; //height of the map
const int width = 150; //width of the map

	//provides forward motion when the forward motion key is pressed
void moveCar(struct car** player, WINDOW* win, bool forward);

	//rotates car in ccw or cw direction when, respectively, A or D are pressed
void rotateCar(struct car** player, WINDOW* win, bool clockwise);

	//determines which key is pressed at a certain moment
void keyPress(struct car** player, WINDOW* win);

	//Places symbols after each completion of the moveCar function and rotateCar functions
void drawCar(WINDOW* win, struct car** player);

	//Creates the race track
void drawMap(WINDOW * win);

	//draws finish line and renews it when cars pass.
void drawFinishLine (WINDOW * win);
	
	//outputs stats such as player color, lap count, etc. 
void outputPlayerStats (struct car** player, WINDOW* win);

	//waits for player to resize the window if it is too small for the game. 
void winSizeCheck (WINDOW* win);

	//since fields due to colour in curses are not simply chars anymore,
		//this function checks if the selected coordinates contain a ' ' or '|'
bool checkMove(WINDOW* win, int y, int x);

	//Once the game is over, output this.
void endScreen (WINDOW* win, struct car** player);


int main(void){
	initscr();
	noecho();
	

    	//10 height lines added for outputting the interface. 
	WINDOW * win = newwin(height + 10, width, 0, 0);

	winSizeCheck(win);

		//creates a rectangular border for the map.	
	noecho();

	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);
	refresh();

	if (!has_colors()) {
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }

    if (start_color() != OK) {
        endwin();
        printf("Did not properly initialize colors\n");
        exit(1);
    }

    start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);

		//sets default colour scheme for the window to be white text with
			//a black background 
	bkgd(MAP_COLOR);
	wrefresh(win);
	refresh();

		//creates the track
	drawMap(win);
	drawFinishLine(win);
	wrefresh(win);


		//creates the car object.
	struct car* player1 = malloc(sizeof(struct car));
	initCar(&player1);
	player1->angle=0;
	player1->head->x = 75;
	player1->head->y = 31;
	player1->mid->x = 74;
	player1->mid->y = 31;
	player1->tail->x = 73;
	player1->tail->y = 31;



		//car is placed according to the coordinates shown in the car object. 
	outputPlayerStats(&player1, win);
	drawCar(win, &player1);
	wrefresh(win);
	refresh();

		//if a key is pressed, calls functions that move the car and read the pressed buttons.
	while(player1->laps < 1){
		keyPress(&player1, win);
		drawCar(win, &player1);

		outputPlayerStats(&player1, win);
		wrefresh(win);
		
		drawFinishLine(win);
	}

	endScreen(win, &player1);

	endwin();
	return 0;
}


void moveCar(struct car** player, WINDOW* win, bool forward){
	int hx = (*player)->head->x;
	int hy = (*player)->head->y;
	int tx = (*player)->tail->x;
	int ty = (*player)->tail->y;

		//depending on the rotarion of the car in steps of 45 degrees, the coordinates are increased in the direction.
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


		//if w is pressed, check is the block in front is an obstacle. Move forward, if safe
	if(forward){
		if(checkMove(win, hy, hx)){
			mvwprintw(win, (*player)->tail->y, (*player)->tail->x, " ");
			(*player)->tail->x = (*player)->mid->x;
			(*player)->tail->y = (*player)->mid->y;
			(*player)->mid->x = (*player)->head->x;
			(*player)->mid->y = (*player)->head->y;
			(*player)->head->x = hx;
			(*player)->head->y = hy;
		}
	}

		//if s is pressed, check is the block behind is an obstacle. Move backward, if safe.
	else{
		if(checkMove(win, ty, tx)){
			mvwprintw(win, (*player)->head->y, (*player)->head->x, " ");
			(*player)->head->x = (*player)->mid->x;
			(*player)->head->y = (*player)->mid->y;
			(*player)->mid->x = (*player)->tail->x;
			(*player)->mid->y = (*player)->tail->y;
			(*player)->tail->x = tx;
			(*player)->tail->y = ty;
		}
	};

		//checks if the car has reached more than 50% of the track so you cannot win by driving on the finish line
		//the finish line is defined at x = 74; firstly checks if the player is in the upper side of the loop
	if ((*player)->mid->x == 74 && (*player)->mid->y <= 11){
		(*player)->midMark = true;
	}	
		//then checks if is on the lower side of the loop, and if they have passed the mid-point
	else if ((*player)->mid->x == 74 && (*player)->mid->y >= height - 10 && (*player)->midMark == true){
		(*player)->laps++;
		(*player)->midMark = false;		
	}

	return;
}

void rotateCar(struct car** player, WINDOW* win, bool clockwise){
		//checks if D is pressed on the keyboard. If true, rotates the car clockwise. 
		//otherwise, it is considered A was pressed, and will turn counter-clockwise.
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

	if(checkMove(win, hy, hx) && checkMove(win, ty, tx)){
		mvwprintw(win, (*player)->head->y, (*player)->head->x, " ");
		mvwprintw(win, (*player)->tail->y, (*player)->tail->x, " ");
		(*player)->head->y = hy;
		(*player)->head->x = hx;
		(*player)->tail->y = ty;
		(*player)->tail->x = tx;
		return;
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

		//wattron intiates the ability to use ncurses formatting properties
			//in this case, the car is outlined and coloured according 
			//to the player.
	wattron(win, A_BOLD);
	wattron(win, PLAYER_ONE_COLOR);
		mvwprintw(win, (*player)->head->y, (*player)->head->x, "0");
		mvwprintw(win, (*player)->mid->y, (*player)->mid->x, "=");
		mvwprintw(win, (*player)->tail->y, (*player)->tail->x, "=");
	wattroff(win, PLAYER_ONE_COLOR);
	wattroff(win, A_BOLD);

}
	//1 = 0*; 2 = 45*; 3 = 90*; 4 = 135*; 5 = 180*; 6 = 225*; 7 = 270*; 8 = 315*
	// ==0		  0			0	    0		0==		     =		    =        =		
	//			 =			=	     =                  =		    =         =
	//			=			=         =                0		    0          0


void drawMap(WINDOW * win){
	double realx, realy;

	wattron(win, A_DIM);

	for (int i = 1; i < width-1; i++){
		mvwhline(win, 1, i, 'x', 1);
		mvwhline(win, height-1, i, 'x', 1);
	}

	for(int y = 2; y < height-1;y++){
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

	wattroff(win, A_DIM);

	return;
}

void drawFinishLine (WINDOW * win){
			// draws finish line back - should move to the drawcar function so that it doesn't glitch
			//(this version might fail if there is a thread where the other car passes the finish line)
		wattron(win, A_DIM);
		for (int i = height-10; i < height-1; i++){
				//should check if car is on the line with an if and the coordinates of the car or any car symbols
			mvwprintw (win, i, 74, "|");
		}
		wattroff(win, A_DIM);

		return;
}

void outputPlayerStats (struct car** player, WINDOW* win){

	mvwprintw(win, height + 2, 10, 
		"Lap count: %d", (*player)->laps);
	
	return;

}

void winSizeCheck (WINDOW* win){
	while (COLS < width || LINES < height+10){
	
		mvwprintw(win, 1, 3,
			"You need to increase the window size.\nTry decreasing character size (ctrl + [-])\nor increasing the terminal window size.");

		if (COLS < width){
			mvwprintw(win, 4, 3, 
				"Window width should increase to %d. Increase it by %d.", width, width - COLS);
		}
		else {
			move(4, 3);
			clrtoeol();
		}
		
		if (LINES < height+10){
			mvwprintw(win, 5, 3, 
				"Window height should increase to %d. Increase it by %d.", height, height - LINES+10);
		}
		else {
			move(5, 3);
			clrtoeol();
		}
		
		mvwprintw(win, 7, 1, 
			"Press any key to check again.");

		refresh();
		wrefresh(win);

		getchar();

	}

	werase(win);

	return;
}

	//A_CHARTEXT lets you parse the character 
		//all while ignoring colours
		//This is also frequently used, so for humans it's 
		//easier to parse this way. 
bool checkMove(WINDOW * win, int y, int x){

	if (((mvinch(y, x) & A_CHARTEXT) == ' ') || 
		((mvinch(y, x) & A_CHARTEXT) == '|')){
			return true;
	}
	else{
		return false;
	}
}

void endScreen(WINDOW* win, struct car** player){
	werase(win);

	wattron (win, A_BOLD);
	mvwprintw(win, 20, 3, 
		"The winner of this game is");

	//checks whose lap count is higher. If it's the thread's, the it 
		// is declared the winner. 
	
	// if player one
	wattron(win, PLAYER_ONE_COLOR);
	mvwprintw(win, 24, 4, 
		"the blue player! ==0");
	wattroff(win, PLAYER_ONE_COLOR);
	// if player two
	wattroff (win, A_BOLD);	

	wattron(win, A_DIM);
		mvwprintw(win, 27, 7, 
		"Press enter to exit the game.");
	wattroff(win, A_DIM);

	wrefresh(win);


	char enter = 0;
	while (enter != '\r' && enter != '\n') {
	 	enter = getchar(); 
	 }

	return;

}