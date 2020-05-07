#ifndef _CONST_HEADER
#define _CONST_HEADER

#include <stdlib.h>		
#include <stdio.h>		
#include <ncurses.h>	
#include <stdbool.h>	
#include <math.h> 		

#include <ctype.h>		
#include <termios.h>	
#include <unistd.h>	


extern const int MAP_HEIGHT = 40;
extern const int MAP_WIDTH  = 150;

extern const float ACCELERATION  = 0.08;
extern const float MAX_SPEED  = 1.0;
extern const float MAX_REV_SPEED  = -0.6;
extern const float TURN_SPEED  = 0.097;
extern const float FRICTION  = 0.03;

void die(const char *s) {
  printf(s);
  exit(1);
}

	//initial settings for terminal
struct termios orig_termios;	

	//taken from the text editor thing. Allows for the term to interpret 
		// char by char, not string after pressing enter.
		//https://github.com/snaptoken/kilo-src/blob/error-handling/kilo.c	

		//de-allocates the resources taken by the window
		//https://pubs.opengroup.org/onlinepubs/007908799/xcurses/endwin.html
void fixTerminal() {
	endwin();
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

#endif