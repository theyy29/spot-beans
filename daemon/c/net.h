#ifndef NET_H_INCLUDED
#define NET_H_INCLUDED

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#include <stdio.h>
#include <string.h>


// Note: need some way for getting statuses and more data.
//  ie. ip the connection came from
//      whether the connection was accepted correctly or not...
class IncomingCallbacks {
protected:
    void *data;

public:
    virtual int read(int fd) = 0;
//    int read(int fd){
//        int len = 2048;
//        char buf[len+1];
//        memset(buf, 0, len+1);
//        int nread = recv(fd, (void *)buf, len, MSG_DONTWAIT);
//        printf("<< read %d bytes. from fd %d. message: >>\n%s", nread, fd, buf);
//        return nread;
//    }

    int preAccept(int){ return 0; } // return -1 to not close the socket.
    void postAccept(int){}

    int preClose(int){ return 0; } // return -1 to not close the socket.
    void postClose(int){}
};

class Incoming {
public:
    Incoming();
    ~Incoming();

    int bindForListen();
    int closeAll();

    int poll();

    // void setAcceptCallback(void (*func)(void *), void *); 
    void setCallbacks(IncomingCallbacks *cb = NULL);

    void addDescriptor(int fd, int events = POLLIN | POLLHUP);
    void removeDescriptor(int fd);
private:
    bool bound;

    int listener;

    // Callback stuff
    IncomingCallbacks *cbs;

    // Stuff for poll
    int fdcount;
    pollfd *fdz;
};

#endif /* ifndef NET_H INCLUDED */
