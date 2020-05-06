#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifndef CONNECT_H_INCLUDED
#define CONNECT_H_INCLUDED

int clientConnect(char* arg1, char* arg2, char* arg3){
    //Connection setup
    char* ptr;
    int cPort = strtol(arg1, &ptr, 10);
    int sPort = strtol(arg3, &ptr, 10);
    if(!cPort || !sPort){
        perror("invalid args");;
    }

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("tcp socket error");
        return -1;
    }
    //set protocol to ipv4 and set port
    struct sockaddr_in myAddr;
    memset(&myAddr, 0, sizeof(struct sockaddr_in));
    myAddr.sin_family = AF_INET; //set family to ipv4
    myAddr.sin_port = htons(cPort); //htons converts to network byte order

    //bind address to socket
    if (bind(clientSocket, (struct sockaddr *) &myAddr, sizeof(struct sockaddr_in)) == -1) {
        perror("tcp bind");
        return -1;
    }

    //server address
    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(sPort);
    inet_aton(arg2, &serverAddr.sin_addr);
    //connect to server address
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr));
    return clientSocket;
}

//name more coherently
typedef struct udpParams{
    player_t* clientPlayer;
    int clientFd;
}udp_params;

typedef struct carMsg{
    int clientFd;
    char button;
    int gameId;
}carmsg_t;

#define PORT     8081 
#define MAXLINE  1024 

// Driver code 
void* clientUdp (void* udpParams) { 
    int sockfd; 
    char buffer[MAXLINE]; 
    struct sockaddr_in     servaddr; 
  
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(1); 
    } 
    
    carmsg_t *carMessage = malloc(sizeof(carmsg_t));
    carMessage -> gameId = ((udp_params*)udpParams)->clientPlayer->gameID;
    carMessage -> clientFd = ((udp_params*)udpParams)->clientFd;


    memset(&servaddr, 0, sizeof(servaddr)); 
      
    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
      
    bool abort = false;
    while (!abort){
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
                    carMessage->button = c;
                    sendto(sockfd, carMessage, sizeof(carmsg_t), 
                    MSG_CONFIRM, (const struct sockaddr *) &servaddr,  
                    sizeof(servaddr)); 
                break;
        
                case 27: //esc
                    abort = true;
                    break;
        
            default:
            break;
        }      
    }

          
    // n = recvfrom(sockfd, (char *)buffer, MAXLINE,  
    //             MSG_WAITALL, (struct sockaddr *) &servaddr, 
    //             &len); 
    // buffer[n] = '\0'; 
    printf("Server : %s\n", buffer); 
  
    close(sockfd); 
    return 0; 
} 


#endif