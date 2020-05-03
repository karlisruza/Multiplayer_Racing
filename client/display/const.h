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
  perror(s);
  exit(1);
}

#endif