
principā gamelist{
      game{ 
          gameid
          playerlist{
              player{player id n other shit}
          }    
      }
}

nuu, tā, šeit actions viss ir client.c
un tad pseido kodā sanāktu
getplayername();
llistgames();
tad tālāk lkm sanāktu
getinput()
un atkarībā no inputa vai nu joingame vai creategame funkcija
šīs funkcijas šādā secībā client.c mainā
aiz connect()

listgames funkcija sūta requestu
requestu ir riktīgi ez aizsūtīt
msg_t message;
message.type = REQUEST_GAME

int length = ((void*)&reply.payload - (void*)&reply.type);

sendData(fd, (void*)&reply, length, NULL);
fd ir clientSocket, kas main pašā sākumā definēts
ui tur kur length reply aizvieto ar message
es vnk copy pasteoju
tad pēc sendData uzreiz
recv(fd, buffer, length, 0);
Alright. Es vienkārši šo pārlasīšu, kad ar to (at)sākšu cīnīties.
jā un pēc recv ir
msg_t* msgr = (msg_t*)buffer;
        handleDataS(msgr, fd);



ir spēle, notiek keyPress
	action
	client thread instancē būs case move car, kas tajā threadā aprēķina
	dabū spēlētāja vietu un leņķi,
	aprēķina jaunās vērtības
	sūtīs updated sarakstu
	
	spēle servera pusē ir simulēta spēle, kas vienk netiek renderēta. 

	spēle posted by playername 
	spēles nosaukums

	spēles izvēle 
	
	//request game (un tad izsauc displayGame)
	// un ielikt loopā
	// 

	input join game vai create game





//get games list
	//join room or create room
//if join lobby 
	//get playerslist
//if player host
	//iespēja force startot iekš lobby
//iekš lobby ir getchar, kas ļauj hostam force startēt
	//event listener, kad pievienojas jauns spēlētājs un/vai force start
//request player tiek izsaukta lobby, tā tiek 
//client pusē ir klausīšanāš funckija inputam, bet paralēli gaida message
//struct paramtri - win, dabū sig, ka sākas spēle, loops jābeidz. 


        //game_t current iekš graphics.h
        //if game list != null
            //if game list head != null
            // while current != null 
                //displayGameList() graphics.h
            //refresh 
            // later getch (r) -> request game no jauna
            // press number atbilstoši spelei, kurai pievienoties
            // c izsauc create game funkciju
            // join game create game rezultāts ir tāds pats
                //display lobby
