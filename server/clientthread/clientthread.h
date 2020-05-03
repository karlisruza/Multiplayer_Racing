#include "../handleData/handleData.h"
#ifndef CLIENTTHREAD_H_INCLUDED
#define CLIENTTHREAD_H_INCLUDED

void *clientThread(void* param){
        char* buffer = (void*)malloc(sizeof(msg_t));
        int length = (sizeof(msg_t));

        while(true){
            int retLen = recv(((params_t*)param)->clientFd, (void*)buffer, length, 0);
            if(retLen < 0){
                printf("fail \n");
            }
            printf("return length: %d\n", retLen);

            msg_t* msgr = (msg_t*)buffer;
            handleData(msgr, (params_t*)param);
        }
}

void printClient(int fd) {
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    if (getpeername(fd, (struct sockaddr *) &addr, &addrLen) == -1) {
        perror("getpeername");
        return;
    }
    printf("%s:%d connected via TCP\n",
	inet_ntoa(addr.sin_addr),
	ntohs(addr.sin_port));
}
#endif