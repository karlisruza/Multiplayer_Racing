#include <ctype.h>		
#include <termios.h>	
#include <unistd.h>	

#ifndef _CONTROLS_HEADER
#define _CONTROLS_HEADER


struct termios orig_termios;	

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

//taken from the text editor thing
void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);

  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

	//determines which key is pressed at a certain moment
bool keyPress(struct car** player, WINDOW* win){
    char c = '\0';
    read(STDIN_FILENO, &c, 1);


	switch (c)
	{
	    case 119: //w
	    case 87:  //W
			moveCar(&(*player), win, true);
	     	break;

	    case 115: //s
	    case 83: //S
	    	moveCar(&(*player), win, false);
	    	break;

	    case 97: //a
	    case 65:  //A
	    	rotateCar(&(*player), win, true);
	     	break;

	    case 100: //d
	    case 68:  //D
	    	rotateCar(&(*player), win, false);
	     	break;

	    case 27: //esc
	    	return false;

		default:
			break;
	}
	return true;
}


char* writePrompt(WINDOW* win, int y, int x){
	const int maxLength = 20;
    char* input = malloc(maxLength+1);
    int length = 0;

    for (int i = 0; i < maxLength; i++){
    	input [i] = ' ';
    }

    while (1){
    	char c = 0;// = getchar();
    	read(STDIN_FILENO, &c, 1);

        if (isalnum(c) && length < maxLength){
            input[length] = c;
            length++;
            displayInput (win, input, y, x);
        } else {
        	if (c == 127 && length != 0) { //backspace
	            length--;
	        	input [length] = ' ';
	        	displayInput (win, input, y, x);
	        }
	        
			if (c == 13 && length > 0){ //enter
				input[length] = '\0';
	        	return input;
	        }
	        
	        if (c == 27){//esc
		    	exit(1);
	        } 
	    }
    }
}


 //
int gameListNav(WINDOW* win, gamelist_t** list){
    char c = '\0';
    int pos = 1;
    int gameCount = (*list)->count;
    gameListNavDraw(win, pos, gameCount);
    int tempID;

    game_t* temp = (game_t*) malloc(sizeof(game_t));
    temp = (*list)->head;

    while (1){
    	char c = 0;// = getchar();
    	read(STDIN_FILENO, &c, 1);

        switch (c){
		    case 'w': //w
		    case 'W':  //W
				if (pos == 1){
					pos = gameCount;
				} 
				else pos--;//go upwards function
				gameListNavDraw(win, pos, gameCount);
		     	break;

		    case 's': //s
		    case 'S': //S
				if (pos == gameCount){
					pos = 1;
				} 
				else pos++;
				gameListNavDraw(win, pos, gameCount);
				break;

			case 'c': //c
		    case 'C': //C
		    	//free current
		    	return 0;

			case 13: //enter
				//enter room/accept selection
				tempID = temp->gameid;
				//free current
		    	return tempID;

		    case 27: //esc
		    	//free current
		    	//return -1;
		    	exit(1);

			default:
				break;
		}
	}


	
	return gameCount;
}

#endif