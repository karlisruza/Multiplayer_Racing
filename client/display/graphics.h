#include "const.h"
#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#define MAP_COLOR     			COLOR_PAIR(1)
#define PLAYER_ONE_COLOR    	COLOR_PAIR(2)
#define PLAYER_TWO_COLOR    	COLOR_PAIR(3)
#define PLAYER_THREE_COLOR    	COLOR_PAIR(4)
#define PLAYER_FOUR_COLOR    	COLOR_PAIR(5)
#define PLAYER_FIVE_COLOR    	COLOR_PAIR(6)
#define PLAYER_SIX_COLOR   		COLOR_PAIR(7)
#define PLAYER_SEVEN_COLOR   	COLOR_PAIR(8)
#define PLAYER_EIGHT_COLOR    	COLOR_PAIR(9)

#define MAP_HEIGHT 				 40;
#define MAP_WIDTH  				150;



	//creates header for the name entering prompt;
void enterName (WINDOW* win){
	clear();
	mvwprintw(win, 1, 1, "ENTER YOUR NAME: ");	
	wrefresh(win);

	move (3,4);
	clrtoeol();

	return;
}
	//displays info written by writePrompt in controls.h
void displayInput (WINDOW* win, char* input, int y, int x){
	wattron (win, A_BOLD);
	mvwprintw(win, y, x, "%s", input);
	wattroff (win, A_BOLD);
	
	wrefresh(win);
	return;
}

	//takes list of games from client.c, and outputs the contents of the list on the screen
void displayGameList (WINDOW * win, gamelist_t** list){
	werase(win);
	int entryCount = 1; 
    if ((*list) != NULL && (*list)->head != NULL){
	    game_t* current = (game_t*)malloc(sizeof(game_t));
	    current = (*list) -> head;	
		mvwprintw(win, 3, 3, "--- LIST OF GAMES ---");

	    while (current != NULL){
	    	wattron(win, A_DIM);
	    	mvwprintw(win, 5*entryCount, 3, "Game number: %d", entryCount);
			mvwprintw(win, 5*entryCount + 1, 6, "Game ID: %d", current->gameid);
			mvwprintw(win, 5*entryCount + 2, 6, "Host ID: %d", current->hostId);
			mvwprintw(win, 5*entryCount + 3, 6, "Status: %d", current->status);
		    wattroff(win, A_DIM);
			current = current -> next;
		    entryCount++;
	    }
	}
	else {
		mvwprintw(win, 3, 3, "--- START THE FIRST GAME ON THIS SERVER ---");
	}
	
	wattron(win, A_BOLD);
	mvwprintw(win, 5*entryCount, 3, "CREATE YOUR OWN GAME (PRESS C)");
	wattroff(win, A_BOLD);
    wrefresh(win);

	return;
}

	//highlights the games from the list
		//called from gameListNav(...) in controls.h
void gameListNavDraw (WINDOW* win, int pos, int maxPos){
	if (pos == 1){ 				//if at the top, dims the bottom and second element
		mvwchgat(win, 5*maxPos 	, 2, -1, A_DIM, MAP_COLOR, NULL);
		mvwchgat(win, 5*(pos+1) , 2, -1, A_DIM, MAP_COLOR, NULL);
	} else if (pos == maxPos){	//if at the bottom, dims top and second to last element
		mvwchgat(win, 5*(pos-1) , 2, -1, A_DIM, MAP_COLOR, NULL);
		mvwchgat(win, 5			, 2, -1, A_DIM, MAP_COLOR, NULL); 
	} else {					//if in the middle, dims the surrounding elements
		mvwchgat(win, 5*(pos+1) , 2, -1, A_DIM, MAP_COLOR, NULL);
		mvwchgat(win, 5*(pos-1) , 2, -1, A_DIM, MAP_COLOR, NULL);
	}

	//highlights the element in pos
	mvwchgat(win, 5*pos 	, 2, -1, A_BOLD, PLAYER_ONE_COLOR, NULL);

	wrefresh(win);
	return;
}

	//draws the lobby, lists the players.
void drawLobby(WINDOW* win, playerlist_t** playerlist, player_t* player){
	playerlist_t* playerList = *playerlist;
    if (playerList != NULL){
       if(playerList->head != NULL){
			werase (win);
			player_t* temp = playerList->head;
			int entryCount = 1;

			mvwprintw (win, 3, 3, "--- GAME LOBBY FOR GAME %d ---", player->gameID);
			while (temp != NULL){
				move(5*entryCount,5);
				clrtoeol();
				move(5*entryCount+1,8);
				clrtoeol();

				wattron(win, A_BOLD);
				wattron(win, COLOR_PAIR(entryCount+1));		//paints the players in their respective car colors. color codes are defined in startGraphics(...) in this file.
				mvwprintw(win, 5*entryCount, 6, "Player name: %s", temp->name); // temp name brokenwattroff(win, COLOR_PAIR(entryCount+1));
				wattroff(win, A_BOLD);

				wattron(win, A_DIM);
				mvwprintw(win, 5*entryCount + 1, 9, "Player ID: %d", temp->ID);
				wattroff(win, A_DIM);
				wattroff(win, COLOR_PAIR(entryCount+1));
				entryCount++;
				temp = temp->next;
			}
			//free temp
			wrefresh(win);
	   }
    }
}
 	//waits for player to resize the window if it is too small for the game. 
		//basically, before the game begins, it checks it the 
		// race track fits on the screen and won't start the 
		// game until everything is order. It's to prevent ncurses glitches.
void winSizeCheck (WINDOW* win){
	while (COLS < MAP_WIDTH || LINES < MAP_HEIGHT+10){
		mvwprintw(win, 1, 3, "You need to increase the window size.\n Try decreasing character size (ctrl + [-])\n or increasing the terminal window size.");

		move(4,3);
		clrtoeol();

		move(5,3);
		clrtoeol();

		//outputs if the terminal width is not enough
		if (COLS < MAP_WIDTH){
			mvwprintw(win, 4, 3, "Window width should increase to %d. Increase it by %d.", 
				MAP_WIDTH, MAP_WIDTH - COLS);
		}
		//outputs if the terminal height is not enough
		if (LINES < MAP_HEIGHT + 10){
			mvwprintw(win, 5, 3, "Window height should increase to %d. Increase it by %d.", 
				MAP_HEIGHT + 10, MAP_HEIGHT - LINES+10);
		}

		mvwprintw(win, 7, 1, "Press [enter] to check again.");
		refresh();
		wrefresh(win);
		//loop for pressing enter
			//basically, without enter, it may glitch 
			//the window launched by ncurses, causing graphics issues.
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
	init_pair(1, COLOR_WHITE, COLOR_BLACK); //map colors
	init_pair(2, COLOR_BLUE, COLOR_BLACK);	//player one
	init_pair(3, COLOR_RED, COLOR_BLACK);	//player two
	init_pair(4, COLOR_GREEN, COLOR_BLACK);	//	...
	init_pair(5, COLOR_YELLOW, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_CYAN, COLOR_BLACK);
	init_pair(8, COLOR_WHITE, COLOR_BLACK);	//player seven
	//init_pair(9, COLOR_RED, COLOR_BLACK);


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

void drawPlayer(WINDOW* win, player_t* player){
	//creates the int value for the car head
	int hx = round((player->x) + 1*cos(player->angle * M_PI));
	int hy = round((player->y) + 1*sin(player->angle * M_PI));
	
	//creates the int value for the car middle
	int mx = round(player->x);
	int my = round(player->y);

	//creates the int value for the car tail
	int tx = round((player->x) - 1*cos(player->angle * M_PI));
	int ty = round((player->y) - 1*sin(player->angle * M_PI));


		
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
	int realx, realy;

	werase(win);
	wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);


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
	
	//
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