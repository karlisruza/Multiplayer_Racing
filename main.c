#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <stdbool.h>
#include <math.h>
#include "car/car.h"
#include <math.h>


/*
[enter]
keycode  28 press
keycode  28 release

w
keycode  17 press
keycode  17 release

a
keycode  30 press
keycode  30 release

s
keycode  31 press
keycode  31 release

d
keycode  32 press
keycode  32 release
*/

#define MAP_COLOR     		COLOR_PAIR(1)
#define PLAYER_ONE_COLOR    COLOR_PAIR(2)
#define PLAYER_TWO_COLOR    COLOR_PAIR(3)
#define fps 				10
#define accel				3
#define plRad				(*player)->angle * M_PI

const int mapHeight = 40;
const int mapWidth = 150;

// CONTROLS FUNCTIONS DECLARATIONS

	//provides forward motion when the forward motion key is pressed
void moveCar(struct car** player, WINDOW* win, bool forward);

	//rotates car in ccw or cw direction when, respectively, A or D are pressed
void rotateCar(struct car** player, WINDOW* win, bool clockwise);

	//since fields due to colour in curses are not simply chars anymore,
		//this function checks if the selected coordinates contain a ' ' or '|'
bool checkMove(WINDOW* win, double y, double x);

	//determines which key is pressed at a certain moment
void keyPress(struct car** player, WINDOW* win);

// GRAPHICS FUNCTIONS DECLARATIONS

	//Places symbols after each completion of the moveCar function and rotateCar functions
void drawCar(WINDOW* win, struct car** player);

	//Creates the race track
void drawMap(WINDOW * win);

	//draws finish line and renews it when cars pass.
void drawFinishLine (WINDOW * win);
	
// UI FUNCTIONS DECLARATIONS
	//outputs stats such as player color, lap count, etc. 
void outputPlayerStats (struct car** player, WINDOW* win);

	//Once the game is over, output this. Basically, congratulates the winner.
void endScreen (WINDOW* win, struct car** player);

	//waits for player to resize the window if it is too small for the game. 
void winSizeCheck (WINDOW* win);
int main(void){
	initscr();
	noecho();

    	//10 mapHeight lines added for outputting the player information. 
	WINDOW * win = newwin(mapHeight + 10, mapWidth, 0, 0);

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
	player1->velocity=0;
	player1->mid->x = 74.0;
	player1->mid->y = 31.0;



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


//######################################
//		   CONTROLS FUNCTIONS 
//######################################

void moveCar(struct car** player, WINDOW* win, bool forward){
	double hx = ((*player)->mid->x) + cos((*player)->angle * M_PI);
	double hy = ((*player)->mid->y) + sin((*player)->angle * M_PI);
	double tx = ((*player)->mid->x) - cos((*player)->angle * M_PI);
	double ty = ((*player)->mid->y) - sin((*player)->angle * M_PI);

		//depending on the rotation of the car in steps of 45 degrees, 
			//the coordinates are increased in the direction.



		//if w is pressed, check is the block in front is an obstacle. Move forward, if safe
	if(forward){
		if(checkMove(win, hy, hx)){
			//mvwprintw(win, (*player)->tail->y, (*player)->tail->x, " ");
			(*player) -> velocity = 1;
			(*player)->mid->x += (*player)->velocity * cos((*player)->angle * M_PI);
			(*player)->mid->y += (*player)->velocity * sin((*player)->angle * M_PI);
		}
	}

		//if s is pressed, check is the block behind is an obstacle. Move backward, if safe.
	else{
		if(checkMove(win, ty, tx)){
			//mvwprintw(win, (*player)->head->y, (*player)->head->x, " ");
			(*player) -> velocity = 1;
			(*player)->mid->x -= (*player)->velocity * cos((*player)->angle * M_PI);
			(*player)->mid->y -= (*player)->velocity * sin((*player)->angle * M_PI);
		}
	};

		//checks if the car has reached more than 50% of the track so you cannot win by driving on the finish line
		//the finish line is defined at x = 74; firstly checks if the player is in the upper side of the loop
	if (round((*player)->mid->x == 74) && (*player)->mid->y <= 11){
		(*player)->midMark = true;
	}	
		//then checks if is on the lower side of the loop, and if they have passed the mid-point
	else if (round((*player)->mid->x == 74) && (*player)->mid->y >= mapHeight - 10 && (*player)->midMark == true){
		(*player)->laps++;
		(*player)->midMark = false;		
	}

	return;
}

void rotateCar(struct car** player, WINDOW* win, bool clockwise){
		//checks if D is pressed on the keyboard. If true, rotates the car clockwise. 
		//otherwise, it is considered A was pressed, and will turn counter-clockwise.
	if(clockwise){
		(*player)-> angle += 0.15;
	}
	else{
		(*player)->angle -= 0.15;
	}
	/*
	double hx = ((*player)->mid->x) + cos((*player)->angle * M_PI);
	double hy = ((*player)->mid->y) + sin((*player)->angle * M_PI);
	double tx = ((*player)->mid->x) - cos((*player)->angle * M_PI);
	double ty = ((*player)->mid->y) - sin((*player)->angle * M_PI);

	//checks whether movement is blocked or not, if not, changes car coordinates

	if(checkMove(win, hy, hx) && checkMove(win, ty, tx)){
		//mvwprintw(win, (*player)->head->y, (*player)->head->x, " ");
		//mvwprintw(win, (*player)->tail->y, (*player)->tail->x, " ");
		//(*player)->angle += 0.1;
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
	}*/
	return;
}


	//A_CHARTEXT lets you parse the character 
		//all while ignoring colours
		//This is also frequently used, so for humans it's 
		//easier to parse this way. 
bool checkMove(WINDOW * win, double y, double x){
	int intx = round(x);
	int inty = round(y);

	if (((mvinch(inty, intx) & A_CHARTEXT) == ' ') || 
		((mvinch(inty, intx) & A_CHARTEXT) == '|')){
			return true;
	}
	
	else{
		return false;
	}
}




//######################################
//        	GRAPHICS FUNCTIONS 
//######################################

void drawCar(WINDOW* win, struct car** player){

		//wattron intiates the ability to use ncurses formatting properties
			//in this case, the car is outlined and coloured according 
			//to the player.

	wattron(win, A_BOLD);
	wattron(win, PLAYER_ONE_COLOR);

		mvwprintw(win, round((*player)->mid->y) - sin((*player)->angle*M_PI), round((*player)->mid->x) - cos((*player)->angle*M_PI), "=");
		mvwprintw(win, round((*player)->mid->y), round((*player)->mid->x), "=");
		mvwprintw(win, round((*player)->mid->y) + sin((*player)->angle*M_PI), round((*player)->mid->x) + cos((*player)->angle*M_PI), "0");

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
		//creates the upper and lower bound.
	for (int i = 1; i < mapWidth-1; i++){
		mvwhline(win, 1, i, 'x', 1);
		mvwhline(win, mapHeight-1, i, 'x', 1);
	}
		//creates the loop of the map
	for(int y = 2; y < mapHeight-1;y++){
		for(int x = 1; x < mapWidth-1; x++){
			realx = x - mapWidth/2;
			realy = (y - mapHeight/2)*3;

			if((realx)*(realx) + (realy)*(realy) > (mapWidth/2) * (mapWidth/2)){
				mvwprintw(win, y, x, "x");
			}
		}
	}
		//creates the middle of the loop
	for(int y = 10; y < mapHeight-10;y++){
		for(int x = 5; x < mapWidth- 8; x++){
			realx = x - mapWidth/2;
			realy = (y - mapHeight/2)*3;

			if((realx)*(realx) + (realy)*(realy) < ((mapHeight)/2) * (mapHeight/2) * 6){
				mvwprintw(win, y, x, "x");
			}
		}
	}

	wattroff(win, A_DIM);

	return;
}

void drawFinishLine (WINDOW * win){
			//pretty much just creates a line of "|" symbols at the
				// starting area of the car
 		wattron(win, A_DIM);
		for (int i = mapHeight-10; i < mapHeight-1; i++){
			mvwprintw (win, i, 74, "|");
		}
		wattroff(win, A_DIM);

		return;
}

/*void drawLine(WINDOW* win, struct coords lineStart, struct coords lineEnd){
	int yStart, xStart;

	yStart = (lineStart->y > lineEnd->y) ? round(lineEnd->y) : round(lineStart->y);
	xStart = (lineStart->x > lineEnd->x) ? round(lineEnd->x) : round(lineStart->x);

//		int hline(chtype ch, int n);
//   	int whline(WINDOW *win, chtype ch, int n);


	//int mvwhline(WINDOW *win, int y, int x, chtype ch, int n);
	int mvwhline(win, int y, int x, chtype ch, int n);

	int mvwvline(WINDOW *win, int y, int x, chtype ch, int n);
	return;

}*/

//######################################
//           UI FUNCTIONS 
//######################################

void outputPlayerStats (struct car** player, WINDOW* win){

	wattron(win, A_BOLD);
	wattron(win, PLAYER_ONE_COLOR);
	mvwprintw(win, mapHeight + 2, 10, 
		"Player one:");
	wattroff(win, PLAYER_ONE_COLOR);
		mvwprintw(win, mapHeight + 4, 10, 
			"Lap count: %d", (*player)->laps);
		mvwprintw(win, mapHeight + 5, 10, 
			"Velocity: %f", (*player)->velocity);
		mvwprintw(win, mapHeight + 6, 10, 
			"Angle: %f", (*player)->angle * M_PI);
		mvwprintw(win, mapHeight + 7, 10, 
			"X axis: %f", (*player)->mid->x);
		mvwprintw(win, mapHeight + 8, 10, 
			"Y axis: %f", (*player)->mid->y);
		
	wattroff(win, A_BOLD);
	
	return;

}


void winSizeCheck (WINDOW* win){
	while (COLS < mapWidth || LINES < mapHeight+10){

		//basically, before the game begins, it checks it the 
			// race track fits on the screen and won't start the 
			// game until everything is order. It's to prevent ncurses glitches. 
	
		mvwprintw(win, 1, 3,
			"You need to increase the window size.\nTry decreasing character size (ctrl + [-])\nor increasing the terminal window size.");

			//outputs if the terminal width is not enough
		if (COLS < mapWidth){
			mvwprintw(win, 4, 3, 
				"Window width should increase to %d. Increase it by %d.", mapWidth, mapWidth - COLS);
		}
		else {
			move(4, 3);
			clrtoeol();	//this removes all chars from the cursor
		}
			//outputs if the terminal height is not enough
		if (LINES < mapHeight+10){
			mvwprintw(win, 5, 3, 
				"Window height should increase to %d. Increase it by %d.", mapHeight + 10, mapHeight - LINES+10);
		}
		else {
			move(5, 3);
			clrtoeol();
		}
		
		mvwprintw(win, 7, 1, 
			"Press [enter] to check again.");

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


//basically, make a thread that constantly keeps reading keypresses. The code underneath takes from showkey.c and some stuff online.


// static void clean_up(void) {
// 	if (ioctl(fd, KDSKBMODE, oldkbmode)) {
// 		perror("KDSKBMODE");
// 		exit(1);
// 	}
// 	if (tcsetattr(fd, 0, &old) == -1)
// 		perror("tcsetattr");
// 	close(fd);
// }


// static void
// get_mode(void) {
//         char *m;
// 	if (ioctl(fd, KDGKBMODE, &oldkbmode)) {
// 		perror("KDGKBMODE");
// 		exit(1);
// 	}
// 	switch(oldkbmode) {
// 	  case K_RAW:
// 	    m = "RAW"; break;
// 	  case K_XLATE:
// 	    m = "XLATE"; break;
// 	  case K_MEDIUMRAW:
// 	    m = "MEDIUMRAW"; break;
// 	  case K_UNICODE:
// 	    m = "UNICODE"; break;
// 	  default:
// 	    m = _("?UNKNOWN?"); break;
// 	}
// 	printf(_("kb mode was %s\n"), m);
// 	if (oldkbmode != K_XLATE) {
// 	    printf(_("[ if you are trying this under X, it might not work\n"
// 		     "since the X server is also reading /dev/console ]\n"));
// 	}
// 	printf("\n");
// }


// int
// main (int argc, char *argv[]) {
// 	const char *short_opts = "haskV";
// 	const struct option long_opts[] = {
// 		{ "help",	no_argument, NULL, 'h' },
// 		{ "ascii",	no_argument, NULL, 'a' },
// 		{ "scancodes",	no_argument, NULL, 's' },
// 		{ "keycodes",	no_argument, NULL, 'k' },
// 		{ "version",	no_argument, NULL, 'V' },
// 		{ NULL, 0, NULL, 0 }
// 	};
// 	int c;
// 	int show_keycodes = 1;
// 	int print_ascii = 0;
// 	struct termios new;
// 	unsigned char buf[16];
// 	int i, n;
// 	set_progname(argv[0]);
// 	setlocale(LC_ALL, "");
// 	bindtextdomain(PACKAGE, LOCALEDIR);
// 	textdomain(PACKAGE);
// 	while ((c = getopt_long(argc, argv,
// 		short_opts, long_opts, NULL)) != -1) {
// 		switch (c) {
// 			case 's':
// 				show_keycodes = 0;
// 				break;
// 			case 'k':
// 				show_keycodes = 1;
// 				break;
// 			case 'a':
// 				print_ascii = 1;
// 				break;
// 			case 'V':
// 				print_version_and_exit();
// 			case 'h':
// 			case '?':
// 				usage();
// 		}
// 	}
// 	if (optind < argc)
// 		usage();
// 	if (print_ascii) {
// 		/* no mode and signal and timer stuff - just read stdin */
// 	        fd = 0;
// 		if (tcgetattr(fd, &old) == -1)
// 			perror("tcgetattr");
// 		if (tcgetattr(fd, &new) == -1)
// 			perror("tcgetattr");
// 		new.c_lflag &= ~ (ICANON | ISIG);
// 		new.c_lflag |= (ECHO | ECHOCTL);
// 		new.c_iflag = 0;
// 		new.c_cc[VMIN] = 1;
// 		new.c_cc[VTIME] = 0;
// 		if (tcsetattr(fd, TCSAFLUSH, &new) == -1)
// 			perror("tcgetattr");
// 		printf(_("\nPress any keys - "
// 		         "Ctrl-D will terminate this program\n\n"));
// 		while (1) {
// 			n = read(fd, buf, 1);
// 			if (n == 1)
// 				printf(" \t%3d 0%03o 0x%02x\n",
// 				       buf[0], buf[0], buf[0]);
// 			if (n != 1 || buf[0] == 04)
// 				break;
// 		}
// 		if (tcsetattr(fd, 0, &old) == -1)
// 			perror("tcsetattr");
// 		exit(0);
// 	}
// 	fd = getfd(NULL);
// 	/* the program terminates when there is no input for 10 secs */
// 	signal(SIGALRM, watch_dog);
// 	/*
// 		if we receive a signal, we want to exit nicely, in
// 		order not to leave the keyboard in an unusable mode
// 	*/
// 	signal(SIGHUP, die);
// 	signal(SIGINT, die);
// 	signal(SIGQUIT, die);
// 	signal(SIGILL, die);
// 	signal(SIGTRAP, die);
// 	signal(SIGABRT, die);
// 	signal(SIGIOT, die);
// 	signal(SIGFPE, die);
// 	signal(SIGKILL, die);
// 	signal(SIGUSR1, die);
// 	signal(SIGSEGV, die);
// 	signal(SIGUSR2, die);
// 	signal(SIGPIPE, die);
// 	signal(SIGTERM, die);
// #ifdef SIGSTKFLT
// 	signal(SIGSTKFLT, die);
// #endif
// 	signal(SIGCHLD, die);
// 	signal(SIGCONT, die);
// 	signal(SIGSTOP, die);
// 	signal(SIGTSTP, die);
// 	signal(SIGTTIN, die);
// 	signal(SIGTTOU, die);
// 	get_mode();
// 	if (tcgetattr(fd, &old) == -1)
// 		perror("tcgetattr");
// 	if (tcgetattr(fd, &new) == -1)
// 		perror("tcgetattr");
// 	new.c_lflag &= ~ (ICANON | ECHO | ISIG);
// 	new.c_iflag = 0;
// 	new.c_cc[VMIN] = sizeof(buf);
// 	new.c_cc[VTIME] = 1;	/* 0.1 sec intercharacter timeout */
// 	if (tcsetattr(fd, TCSAFLUSH, &new) == -1)
// 		perror("tcsetattr");
// 	if (ioctl(fd, KDSKBMODE, show_keycodes ? K_MEDIUMRAW : K_RAW)) {
// 		perror("KDSKBMODE");
// 		exit(1);
// 	}
// 	printf(_("press any key (program terminates 10s after last keypress)...\n"));
// 	while (1) {
// 		alarm(10);
// 		n = read(fd, buf, sizeof(buf));
// 		for (i = 0; i < n; i++) {
// 			if (!show_keycodes)
// 				printf("0x%02x ", buf[i]);
// 			else
// 				printf(_("keycode %3d %s\n"),
// 					buf[i] & 0x7f,
// 					buf[i] & 0x80 ? _("release")
// 				                      : _("press"));
// 		}
// 		if (!show_keycodes)
// 			printf("\n");
// 	}
// 	clean_up();
// 	exit(0);
// }