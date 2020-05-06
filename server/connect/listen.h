#ifndef LISTEN_H_DEFINED
#define LISTEN_H_DEFINED
#define PORT 8050
#define _MAX_LISTEN_QUEUE 10

int startListen(void){
    int serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        perror("tcp socket error");
        return -1;
    }
    
    //set protocol to ipv4 and set port
    struct sockaddr_in myAddr;
    memset(&myAddr, 0, sizeof(struct sockaddr_in));
    myAddr.sin_family = AF_INET;
    myAddr.sin_port = htons(PORT); //htons converts to network byte order

    //bind address to socket
    if (bind(serverFd, (struct sockaddr *) &myAddr, sizeof(struct sockaddr_in)) == -1) {
        perror("tcp bind");
        return -1;
    }

    //set socket to listen
    if (listen(serverFd, _MAX_LISTEN_QUEUE) == -1) {
        perror("listen");
        return -1;
    }
    printf("Listening on port %d \n", PORT);
    return serverFd;
}

#define PORT_UDP     8081 
#define MAXLINE 1024 
  
typedef struct carMsg{
    int clientFd;
    char button;
    int gameId;
}carmsg_t;

// Driver code 
void* serverUdp(void* param) { 
    int sockfd; 
    char buffer[MAXLINE]; 
    struct sockaddr_in servaddr, cliaddr; 
      
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    memset(&servaddr, 0, sizeof(servaddr)); 
    memset(&cliaddr, 0, sizeof(cliaddr)); 
      
    // Filling server information 
    servaddr.sin_family    = AF_INET; // IPv4 
    servaddr.sin_addr.s_addr = INADDR_ANY; 
    servaddr.sin_port = htons(PORT_UDP); 
      
    // Bind the socket with the server address 
    if ( bind(sockfd, (const struct sockaddr *)&servaddr,  
            sizeof(servaddr)) < 0 ) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
      
    int len, n; 
    carmsg_t *carMessage = malloc(sizeof(carmsg_t)); 
    len = sizeof(cliaddr);  //len is value/resuslt 
    while (1){
        n = recvfrom(sockfd, carMessage, sizeof(carmsg_t),  
                    MSG_WAITALL, ( struct sockaddr *) &cliaddr, 
                    &len); 
        buffer[n] = '\0'; 
        printf("char : %c    fd : %d    game : %d\n", carMessage->button,
                        carMessage->clientFd, carMessage->gameId);
    } 
    // sendto(sockfd, (const char *)hello, strlen(hello),  
    //     MSG_CONFIRM, (const struct sockaddr *) &cliaddr, 
    //         len); 
    // printf("Hello message sent.\n");  
      
    return 0; 
} 

#endif
