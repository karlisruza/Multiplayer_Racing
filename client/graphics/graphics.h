#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#define MAP_COLOR     		COLOR_PAIR(1)
#define PLAYER_ONE_COLOR    COLOR_PAIR(2)
#define PLAYER_TWO_COLOR    COLOR_PAIR(3)

#define MAP_HEIGHT          40
#define MAP_WIDTH           150


WINDOW* winSetup(void){
	initscr();
	noecho();
    //10 mapHeight lines added for outputting the player information. 
	WINDOW * win = newwin(MAP_HEIGHT + 10, MAP_WIDTH, 0, 0);
	return win;
}

//didn't figure out how to allow one to write - you can't use backspace yet or 
//see what you type. Otherwise, works just fine for now while i'm still figuring
// out the other terminal system.
char* enterNameMenu (WINDOW* win){
		mvwprintw(win, 0, 0, "ENTER YOUR NAME: ");	
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
	 		if (enter == '\r' || enter == '\n') break; 
	 		userName[length] = enter;
			usableName = true;
			printw("%c", enter);
			refresh();
	 	};

	 	userName[length] = '\0';

	 	if (!usableName){
	 		return "";
	 	}

		werase(win);
		return userName;
}


void winSizeCheck (WINDOW* win){
	while (COLS < MAP_WIDTH || LINES < MAP_HEIGHT+10){
		//basically, before the game begins, it checks it the 
		// race track fits on the screen and won't start the 
		// game until everything is order. It's to prevent ncurses glitches. 
	
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

#endif