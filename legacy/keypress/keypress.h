void keyPress(int playerid, int gameid, int fd){
	int key = getch();
    msg_t message;
    message.type = UP;

    up_pt data;
    data.playerID = playerid;
    data.gameID = gameid;

    action_t keypress; //(0, 1) UP (0, -1) Down (1, 0) Right (-1, 0) Left
	switch (key)
	{
	    case 'w':
            keypress.x = 0;
            keypress.y = 1;
	     	break;
	    case 's':
            keypress.x = 0;
            keypress.y = -1;
	    	break;
	    case 'a':
            keypress.x = -1;
            keypress.y = 0;
	     	break;
	    case 'd':
            keypress.x = 1;
            keypress.y = 0;
	     	break;
	}
    data.action = keypress;
    memcpy((void*)&message.payload, (void*)&data, sizeof(data));
    int length = ((void*)&message.payload - (void*)&message.type) + sizeof(data);
    sendData(fd, (void*)&message, length, NULL);
}