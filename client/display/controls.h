#include <ctype.h>		
#include <termios.h>	
#include <unistd.h>	

#ifndef _CONTROLS_HEADER
#define _CONTROLS_HEADER

	//initial settings for terminal
struct termios orig_termios;	

	//taken from the text editor thing. Allows for the term to interpret 
		// char by char, not string after pressing enter.
		//https://github.com/snaptoken/kilo-src/blob/error-handling/kilo.c	
void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  atexit(disableRawMode);

//http://man7.org/linux/man-pages/man3/termios.3.html -- Raw mode
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}



	//basically, on each keypress, a character is added to the
		//user's input. enter to accept. Max length is 20. 
		//Can be reused wherever prompts are needed.
char* writePrompt(WINDOW* win, int y, int x, player_t** clientPlayer){
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
				strcpy((*clientPlayer)->name, input);
	        	return input;
	        }
	        
	        if (c == 27){//esc
		    	exit(1);
	        } 
	    }
    }
}


 	//navigator function during gameList
		//w or s to go up or down; use c to create room
int gameListNav(WINDOW* win, gamelist_t** list, player_t** player, int clientFd){
    player_t* clientPlayer = *player;
	char c = '\0';
    int pos = 1;
    int gameCount = (*list)->count; //err check needed
    gameListNavDraw(win, pos, gameCount);
    int tempID;
	game_t* temp;
    while (1){
    	char c = 0;// = getchar();
    	read(STDIN_FILENO, &c, 1);
        switch (c){
		    case 'w': //w
		    case 'W':  //W
				if (pos == 1){
					pos = gameCount;
				} else {
					pos--;//go upwards function
				}
				gameListNavDraw(win, pos, gameCount);
			    // wattron(win, A_DIM);
	    		// mvwprintw(win, 5, 40, "Game number: %d", pos);
				// mvwprintw(win, 5 + 1, 40, "Game ID: %d", temp->gameid);
			    // mvwprintw(win, 5 + 2, 40, "Host ID: %d", temp->hostId);
			    // wattroff(win, A_DIM);
				wrefresh(win);
		     	break;
		    case 's': //s
		    case 'S': //S
				if (pos == gameCount){
					pos = 1;
					temp = (*list)->head;
				} 
				else {
					pos++;
				}
				gameListNavDraw(win, pos, gameCount);
				// wattron(win, A_DIM);
	    		// mvwprintw(win, 5, 40, "Game number: %d", pos);
				// mvwprintw(win, 5 + 1, 40, "Game ID: %d", temp->gameid);
				// mvwprintw(win, 5 + 2, 40, "Host ID: %d", temp->hostId);
				// mvwprintw(win, 5 + 3, 40, "Status:  %d", temp->status);
				// wattroff(win, A_DIM);
				wrefresh(win);
				break;

			case 'c': //c
		    case 'C':{ //C
				if(createGame(&clientPlayer, clientFd) < 0){
					perror("create game failed\n");
					exit(1);
				}
				else{
					return 0;
				}
			}
			case 13:{ //enter
				// clientPlayer->gameID = temp->gameid;
				// return 0;
				int counter = 1;
				game_t* temp = (*list)->head;
				while(temp != NULL){
					if(counter == pos){
						clientPlayer->gameID = temp->gameid;
						return 0;
					}
					counter++;
					temp = temp->next;
				}
		    	return 0;
			}
		    case 27: //esc
		    	//free current
		    	//return -1;
		    	exit(1);
			// case 'q':
			// case 'Q:'
			default:
				break;
		}
	}
}


	//incomplete at the moment but enter to force start, backspace to leave
void lobbyControls(WINDOW* win){
    char c = '\0';
    int pos = 1;

    while (1){
    	char c = 0;// = getchar();
    	read(STDIN_FILENO, &c, 1);

        switch (c){

			case 'r': //c
		    case 'R': //C
		    	//refresh lobby
		    	return;

			case 127: //backspace
				//leave room
				//free current
		    	return;

			case 13: //enter
				//launch game
		    	return;		    	

		    case 27: //esc
		    	//free current
		    	//return -1;
		    	exit(1);

			default:
				break;
		}
	}

	return;
}


//name more coherently
typedef struct tparams{
    player_t* clientPlayer;
    int clientFd;
    bool isHost;
}tparams_t;

void *userInput(void* params){ 
    int clientFd = ((tparams_t*)params)->clientFd;
    player_t* clientPlayer = ((tparams_t*)params)->clientPlayer;

	while (1){
		char c = '\0';
    	read(STDIN_FILENO, &c, 1);

		switch (c)
		{
		    case 13:  //ent
		    	printf("enter was pressed");
		    	if (((tparams_t*)params)->isHost){
		    		requestGameStart(clientPlayer, clientFd);
		    		pthread_exit(NULL);
		    	}
		    	break;

		    case 27: //esc
		    	if (((tparams_t*)params)->isHost){
		    		//if is host, delete Lobby?
		    	} else {
		    		//exitLobby
		    	}
		    	pthread_exit(NULL);
		    	break;


			default:
				printf("%c\n", c);
				break;
		}
	}
}

void *lobbyInput(void* params){ 
    int clientFd = ((tparams_t*)params)->clientFd;
    player_t* clientPlayer = ((tparams_t*)params)->clientPlayer;

	while (1){
		char c = '\0';
    	read(STDIN_FILENO, &c, 1);

		switch (c)
		{
		    case 13:  //ent
		    	printf("enter was pressed");
		    	if (((tparams_t*)params)->isHost){
		    		requestGameStart(clientPlayer, clientFd);
		    		pthread_exit(NULL);
		    	}
		    	break;

		    case 27: //esc
		    	if (((tparams_t*)params)->isHost){
		    		//if is host, delete Lobby?
		    	} else {
		    		//exitLobby
		    	}
		    	pthread_exit(NULL);
		    	break;


			default:
				// printf("%c\n", c);
				break;
		}
	}
}



	//new car function that is a thread
char carControl (){

		char c = '\0';
		read(STDIN_FILENO, &c, 1);
	
	
		switch (c)
		{
		    case 119: //w
		    case 87:  //W
			case 115: //s
		    case 83: //S
		    case 97: //a
		    case 65:  //A
		    case 100: //d
		    case 68:  //D
				return c;
	    		break;
	
		    case 27: //esc
				//but also send exit signal
				return 0;	

		    default:
		    return 0;
		}
}

	//legacy car function that determines which wasd key was pressed
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

#endif 