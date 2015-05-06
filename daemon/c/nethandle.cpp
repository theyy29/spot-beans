#include "nethandle.h"

#include "queue/command.h"

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>

// int read(int fd, void *data);
// int preAccept(int fd, void *data);
// void postAccept(int fd, void *data);
// int preClose(int fd, void *data);
// void postClose(int fd, void *data);

NetHandle::NetHandle(Queue *q){
    i = new Incoming();
    this->q = q;

//    this->read       = ::read;
//    this->preAccept  = ::preAccept;
//    this->postAccept = ::postAccept;
//    this->preClose   = ::preClose;
//    this->postClose  = ::postClose;

    i->setCallbacks(this);
    i->bindForListen();
}

NetHandle::~NetHandle(){
    if(i) delete i;
}

int NetHandle::poll(){
    return i->poll();
}

int NetHandle::read(int fd){
    printf("<< NetHandle::read() >>\n");

    char buf[1025];
    memset(buf, 0, 1025);
    
    int nread = recv(fd, buf, 1024, MSG_DONTWAIT);

    //printf("||%s||\n", buf);
    
    // At this point, there would be a possible new thread spawned (or just a new thing added to the queue the worker thread handles)

    if(nread > 0){
        std::cout << "queuing new command\n";
        Command *c = new Command(buf);
        q->queue(c);
    }

    return nread;
}

int NetHandle::preAccept(int fd){
    printf("<< preAccept() >>\n");
    return 0;
}

void NetHandle::postAccept(int fd){
    printf("<< postAccept() >>\n");
}

int NetHandle::preClose(int fd){
    printf("<< preClose() >>\n");
    return 0;
}

void NetHandle::postClose(int fd){
    printf("<< postClose() >>\n");
}
