#ifndef PING_H_INCLUDED
#define PING_H_INCLUDED

void ping(int clientFd){
        msg_t reply;
        reply.type = PONG;
        int length = ((void*)&reply.payload - (void*)&reply.type);
        sendData(clientFd, (void*)&reply, length, NULL);
        return;
}

#endif // !1