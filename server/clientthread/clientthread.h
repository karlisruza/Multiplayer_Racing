#include "../handleData/handleData.h"
#ifndef CLIENTTHREAD_H_INCLUDED
#define CLIENTTHREAD_H_INCLUDED

void *clientThread(void* param){
        char* buffer = (void*)malloc(sizeof(msg_t));
        int length = (sizeof(msg_t));
        pthread_mutex_t* lock = ((params_t*)param)->lock;

        while(true){
            int retLen = recv(((params_t*)param)->clientFd, (void*)buffer, length, 0);
            if(retLen < sizeof(int)){
                printf("fail \n");
                return NULL;
            }
            msg_t* msgr = (msg_t*)buffer;
            pthread_mutex_lock(lock);
            handleData(msgr, (params_t*)param);
            pthread_mutex_unlock(lock);
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