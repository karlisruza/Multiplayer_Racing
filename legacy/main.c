

#include "../client/display/const.h"
#include "../client/display/car.h"
#include "../client/display/graphics.h"
#include "../client/display/controls.h"



#define MAP_COLOR     		COLOR_PAIR(1)
#define PLAYER_ONE_COLOR    COLOR_PAIR(2)
#define PLAYER_TWO_COLOR    COLOR_PAIR(3)



int main(void){
	enableRawMode();

    	//10 MAP_HEIGHT lines added for outputting the player information. 
	WINDOW * win = startGraphics();


	drawMap(win);
	drawFinishLine(win);
	wrefresh(win);

	struct car* player1 = initPlayer();

		//car is placed according to the coordinates shown in the car object. 
	playerStats(&player1, win);
	drawCar(win, &player1);

	wrefresh(win);
	enableRawMode();

	bool run = true;
		//if a key is pressed, calls functions that move the car and read the pressed buttons.
	while(player1->laps < 2 && run){
		
		//natural deceleration	
		//consider deceleration with division rather than linearly

		intertia(&player1, win);

		//check for lap crossing
		checkLapCrosing(&player1, win);

	    run = keyPress(&player1, win);
		
		drawCar(win, &player1);

		playerStats(&player1, win);

		wrefresh(win);

		
		drawFinishLine(win);
	}
		//once lap is finished, launches endScreen displaying the winner.

	if (player1 -> laps == 2){		
		endScreen(win, &player1);
	}

	endwin();
	return 0;
}




//NEED TO CHECK FINISH LINE SOMEHOW - EITHER RECIEVE VIA MAP PACKET OR 
	// CREATE A FUNCTION THAT GOES OVER THE WINDOW DIAGONALLY TO MORE 
	// OR LESS FIGURE OUT WHERE THE LINE IS, AND PUT THE MIDMARK ON THE OPPOSITE SIDE.

// void checkLapCrosing(struct car** player, WINDOW* win){
// 		if ((*player)->mid->x >= 73.0 && (*player)->mid->x >= 74.0){

// 			if ((*player)->mid->y <= 11.0){
// 				(*player)->midMark = true; 
// 			} else if ((*player)->mid->y >= MAP_HEIGHT - 10.0 && (*player)->midMark == true){
// 				(*player)->laps++;
// 				(*player)->midMark = false;		
// 			};
// 		};
// }



//######################################
//        	GRAPHICS FUNCTIONS 
//######################################





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