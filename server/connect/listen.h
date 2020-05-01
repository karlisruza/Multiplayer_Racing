#ifndef LISTEN_H_DEFINED
#define LISTEN_H_DEFINED
#define PORT 8012
#define _MAX_LISTEN_QUE 10

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
    if (listen(serverFd, _MAX_LISTEN_QUE) == -1) {
        perror("listen");
        return -1;
    }
    printf("Listening on port %d \n", PORT);
    return serverFd;
}

#endif
