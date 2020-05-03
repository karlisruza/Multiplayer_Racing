#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#define MAP_COLOR     		COLOR_PAIR(1)
#define PLAYER_ONE_COLOR    COLOR_PAIR(2)
#define PLAYER_TWO_COLOR    COLOR_PAIR(3)



WINDOW* winSetup(void){
	initscr();
	noecho();
    //10 MAP_HEIGHT lines added for outputting the player information. 
	WINDOW * win = newwin(MAP_HEIGHT + 10, MAP_WIDTH, 0, 0);
	return win;
}

//get games list
	//join room or create room

//if join lobby 
	//get playerslist

//if player host
	//iespēja force startot iekš lobby

//iekš lobby ir getchar, kas ļauj hostam force startēt
	//event listener, kad pievienojas jauns spēlētājs un/vai force start

//request player tiek izsaukta lobby, tā tiek 

//client pusē ir klausīšanāš funckija inputam, bet paralēli gaida message


//struct paramtri - win, dabū sig, ka sākas spēle, loops jābeidz. 






//didn't figure out how to allow one to write - you can't use backspace yet or 
//see what you type. Otherwise, works just fine for now while i'm still figuring
// out the other terminal system.
char* enterNameMenu (WINDOW* win){
		mvwprintw(win, 1, 1, "ENTER YOUR NAME: ");	
		refresh();
		wrefresh(win);
		move(3,4);

		char* userName = malloc(20);
		bool usableName = false;
		char enter = 0;

		int length;
		for (length = 0; length < 20; length++){
			enter = getchar();
			//if (!alnum(enter)) usableName = false;
	 		if (enter == '\r' || enter == '\n') {userName [length] = '\0'; break;} 
	 		userName[length] = enter;
			usableName = true;
			printw("%c", enter);
			refresh();
	 	};

	 	if (!usableName){
	 		return "";
	 	}

		werase(win);
		return userName;
}

void displayGameList (WINDOW * win, gamelist_t** list, int clientFd){
	    werase(win);
	    requestGame(list, clientFd);
	    game_t* current = (*list)->head;

	    for (int i = 0; i < (*list)->count; i++){

	    	mvwprintw(win, 4*i, 3, "ID: %d", current->gameid);

	    	current = current->next;
	    	// game->curr = gameData->hostId;
	        // game->status = gameData->status;
	    }
	    wrefresh(win);
	    char enter;
	    while (enter != '\n'){
	    	enter = getchar();
	    }
	 return;
}

 	//waits for player to resize the window if it is too small for the game. 
		//basically, before the game begins, it checks it the 
		// race track fits on the screen and won't start the 
		// game until everything is order. It's to prevent ncurses glitches.
void winSizeCheck (WINDOW* win){
	while (COLS < MAP_WIDTH || LINES < MAP_HEIGHT+10){
 
	
		mvwprintw(win, 1, 3, "You need to increase the window size.\nTry decreasing character size (ctrl + [-])\nor increasing the terminal window size.");
		//outputs if the terminal width is not enough
		if (COLS < MAP_WIDTH){
			mvwprintw(win, 4, 3, "Window width should increase to %d. Increase it by %d.", MAP_WIDTH, MAP_WIDTH - COLS);
		}
		else {
			move(4, 3);
			clrtoeol();	//this removes all chars from the cursor
		}
		//outputs if the terminal height is not enough
		if (LINES < MAP_HEIGHT + 10){
			mvwprintw(win, 5, 3, "Window height should increase to %d. Increase it by %d.", MAP_HEIGHT + 10, MAP_HEIGHT - LINES+10);
		}
		else {
			move(5, 3);
			clrtoeol();
		}
		mvwprintw(win, 7, 1, "Press [enter] to check again.");
		refresh();
		wrefresh(win);
		//loop for pressing enter
		char enter = 0;
		while (enter != '\r' && enter != '\n') {
	 		enter = getchar(); 
	 	};
	}
	werase(win);
	return;
}

//After the game is done, displays the winner and directs the player back to the lobby.

void endScreen(WINDOW* win, struct car** player){

	werase(win);

	wattron (win, A_BOLD);
	mvwprintw(win, 20, 3, 
		"The winner of this game is");

	//checks whose lap count is higher or equals exactly 3
		//should have enum for the player colours and player identifiers 
	
	//outputs info about the winner.
	wattron(win, PLAYER_ONE_COLOR);
	mvwprintw(win, 24, 4, 
		"the blue player! ==0");

	wattroff(win, PLAYER_ONE_COLOR);
	wattroff (win, A_BOLD);	

		// if player one
	//gets player list, finds the player who first reached 3 laps
		//wattron player_color and bold
		//mvwprintw(win, 24, 4, "%s ==0", player->playerName);
		//wattroff player_color and bold

	wattron(win, A_DIM);
		mvwprintw(win, 27, 7, 
		"Press enter to return to the lobby.");
	wattroff(win, A_DIM);

	wrefresh(win);


	char enter = 0;
	while (enter != '\r' && enter != '\n') {
	 	enter = getchar(); 
	 }

	return;
}

 	//creates window instance as well as initializes what ncurses needs.
WINDOW* startGraphics (){
	initscr();

	WINDOW* win = newwin(MAP_HEIGHT + 10, MAP_WIDTH, 0, 0);
	winSizeCheck(win);

	//creates a rectangular border for the map.	
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
	refresh();

	return win;
}

	//Places symbols after each completion of the moveCar function and rotateCar functions
void drawCar(WINDOW* win, struct car** player){

		//creates the int value for the car head
	int hx = round(((*player)->mid->x) + 1*cos((*player)->angle * M_PI));
	int hy = round(((*player)->mid->y) + 1*sin((*player)->angle * M_PI));
	
		//creates the int value for the car middle
	int mx = round((*player)->mid->x);
	int my = round((*player)->mid->y);

		//creates the int value for the car tail
	int tx = round(((*player)->mid->x) - 1*cos((*player)->angle * M_PI));
	int ty = round(((*player)->mid->y) - 1*sin((*player)->angle * M_PI));


		
		//wattron intiates the ability to use ncurses formatting properties
			//in this case, the car is outlined and coloured according 
			//to the player.
	wattron(win, A_BOLD);
	wattron(win, PLAYER_ONE_COLOR);

		mvwprintw(win, ty, tx, "=");
		mvwprintw(win, my, mx, "=");
		mvwprintw(win, hy, hx, "0");

	wattroff(win, PLAYER_ONE_COLOR);
	wattroff(win, A_BOLD);

}

	//Creates the race track
void drawMap(WINDOW * win){
	double realx, realy;

	wattron(win, A_DIM);
		//creates the upper and lower bound.
	for (int i = 1; i < MAP_WIDTH-1; i++){
		mvwhline(win, 1, i, 'x', 1);
		mvwhline(win, MAP_HEIGHT-1, i, 'x', 1);
	}
		//creates the loop of the map
	for(int y = 2; y < MAP_HEIGHT-1;y++){
		for(int x = 1; x < MAP_WIDTH-1; x++){
			realx = x - MAP_WIDTH/2;
			realy = (y - MAP_HEIGHT/2)*3;

			if((realx)*(realx) + (realy)*(realy) > (MAP_WIDTH/2) * (MAP_WIDTH/2)){
				mvwprintw(win, y, x, "x");
			}
		}
	}
		//creates the middle of the loop
	for(int y = 10; y < MAP_HEIGHT-10;y++){
		for(int x = 5; x < MAP_WIDTH- 8; x++){
			realx = x - MAP_WIDTH/2;
			realy = (y - MAP_HEIGHT/2)*3;

			if((realx)*(realx) + (realy)*(realy) < ((MAP_HEIGHT)/2) * (MAP_HEIGHT/2) * 6){
				mvwprintw(win, y, x, "x");
			}
		}
	}

	wattroff(win, A_DIM);

	return;
}

	//draws finish line and renews it when cars pass.
void drawFinishLine (WINDOW * win){
 		wattron(win, A_DIM);
		for (int i = MAP_HEIGHT-10; i < MAP_HEIGHT-1; i++){
			mvwprintw (win, i, 74, "|");
		}
		wattroff(win, A_DIM);

		return;
}

void playerStats (struct car** player, WINDOW* win){

	wattron(win, A_BOLD);
	wattron(win, PLAYER_ONE_COLOR);
	mvwprintw(win, MAP_HEIGHT + 2, 10, 
		"Player one:");
	wattroff(win, PLAYER_ONE_COLOR);
		mvwprintw(win, MAP_HEIGHT + 4, 10, 
			"Lap count: %d", (*player)->laps);
		
		mvwprintw(win, MAP_HEIGHT + 6, 10, 
			"midMark: %d", (*player)->midMark);

	wattroff(win, A_BOLD);
	
	return;
}

	//outputs stats such as player color, lap count, etc. 
void playerDebugStats (struct car** player, WINDOW* win){

	wattron(win, A_BOLD);
	wattron(win, PLAYER_ONE_COLOR);
	mvwprintw(win, MAP_HEIGHT + 2, 30, 
		"Player one:");
	wattroff(win, PLAYER_ONE_COLOR);
		mvwprintw(win, MAP_HEIGHT + 4, 30, 
			"Lap count: %d", (*player)->laps);
		mvwprintw(win, MAP_HEIGHT + 5, 30, 
			"Velocity: %f", (*player)->velocity);
		mvwprintw(win, MAP_HEIGHT + 6, 30, 
			"Angle: %f", (*player)->angle * M_PI);
		mvwprintw(win, MAP_HEIGHT + 7, 30, 
			"X axis: %f", (*player)->mid->x);
		mvwprintw(win, MAP_HEIGHT + 8, 30, 
			"Y axis: %f", (*player)->mid->y);

		//head
	double hx = ((*player)->mid->x) + 1*cos((*player)->angle * M_PI);
	double hy = ((*player)->mid->y) + 1*sin((*player)->angle * M_PI);
	//tail
	double tx = ((*player)->mid->x) - 1*cos((*player)->angle * M_PI);
	double ty = ((*player)->mid->y) - 1*sin((*player)->angle * M_PI);

		mvwprintw(win, MAP_HEIGHT + 4, 50, 
			"hx: %f", hx);
		mvwprintw(win, MAP_HEIGHT + 5, 50, 
			"hy: %f", hy);
		mvwprintw(win, MAP_HEIGHT + 6, 50, 
			"tx: %f", tx);
		mvwprintw(win, MAP_HEIGHT + 7, 50, 
			"ty: %f", ty);
		
	wattroff(win, A_BOLD);
	
	return;
}

#endif