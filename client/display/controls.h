#include <ctype.h>		
#include <termios.h>	
#include <unistd.h>	

#ifndef _CONTROLS_HEADER
#define _CONTROLS_HEADER

	//taken from the text editor thing. Allows for the term to interpret 
		// char by char, not string after pressing enter.
		//https://github.com/snaptoken/kilo-src/blob/error-handling/kilo.c	

void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr failed\n");

//http://man7.org/linux/man-pages/man3/termios.3.html -- Raw mode
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN); //| ISIG);

  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr failed\n");
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
    int gameCount = (*list)->count; 

    bool allowNav;
    if ((*list) != NULL && (*list)->head != NULL){
    	allowNav=true;
    } else {
    	allowNav=false;
    }

    gameListNavDraw(win, pos, gameCount);
    int tempID;
	game_t* temp;
    while (1){
    	char c = 0;// = getchar();
    	read(STDIN_FILENO, &c, 1);

        switch (c){
		    case 'w': //w
		    case 'W':  //W
		    	if(allowNav){
		    			if (pos == 1){
		    				pos = gameCount;
		    			} else {
		    				pos--;//go upwards function
		    			}
		    			gameListNavDraw(win, pos, gameCount);
		    			wrefresh(win);
		    		}
		     	break;
		    case 's': //s
		    case 'S': //S
		    	if (allowNav){
					if (pos == gameCount){
						pos = 1;
						temp = (*list)->head;
					} 
					else {
						pos++;
					}
					gameListNavDraw(win, pos, gameCount);
					wrefresh(win);
				}
				break;

				//if C is pressed, creates game and subsequently joins it
			case 'c': //c
		    case 'C':{ //C
				if(createGame(&clientPlayer, clientFd) < 0){
					die("create game failed\n");
				}
				else{
					return 0;
				}
			}
			case 13:{ //enter

				//if no games are loaded, enter will create a game
				if (!allowNav){
					if(createGame(&clientPlayer, clientFd) < 0){
						die("create game failed\n");
					} else {
						return 0;
					}
				}

				//otherwise, joins a game
				int counter = 1;
				game_t* temp = (*list)->head;
					//goes through list until the game within the list is found
				while(counter != pos){
					counter++;
					temp = temp->next;
				}	
					//if the ponter turns out to be null, returns -1 for error
				if (temp == NULL){
					die("faulty game list; chosen game does not exist\n");
				} else { 
					//makes the gameID for the player the temp game ID
					(*player)->gameID = temp->gameid;
					return 1;
				}
			}
		    case 27: //esc
		    	die("exitted\n");
		    	return -1;

			default:
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
		    	// printf("enter was pressed");
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


			default:
				// printf("%c\n", c);
				break;
		}
	}
}



	//new car function that is a thread
void* carControl (void* params){
		printf("thread launch");
	    int clientFd = ((tparams_t*)params)->clientFd;
	    player_t* clientPlayer = ((tparams_t*)params)->clientPlayer;
		playerlist_t* list = ((tparams_t*)params)->playerList;
	

		while (1){	
			char c = '\0';
			read(STDIN_FILENO, &c, 1);
	
			switch (c){
				    case 119: //w
				    case 87:  //W
				    	requestUpdatePlayer(&list, clientPlayer, clientFd, 0, 1);
				    	break;

					case 115: //s
				    case 83: //S
						requestUpdatePlayer(&list, clientPlayer, clientFd, 0, -1);
				    	break;

				    case 97: //a
				    case 65:  //A
						requestUpdatePlayer(&list, clientPlayer, clientFd, 1, 0);
				    	break;

				    case 100: //d
				    case 68:  //D
						requestUpdatePlayer(&list, clientPlayer, clientFd, -1, 0);
				    	break;
			
				    case 27: //esc
						//but also send exit signal
						pthread_exit(NULL);		

				    default:
				    	break;
				    	//return 0;
			}
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
