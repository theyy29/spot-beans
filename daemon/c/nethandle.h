#ifndef NETHANDLE_H_INCLUDED
#define NETHANDLE_H_INCLUDED

#include "net.h"
#include "queue/queue.h"

class NetHandle: IncomingCallbacks {
public:
    NetHandle(Queue *q);
    ~NetHandle();

    int poll();

    int read(int fd);
    int preAccept(int fd); // return -1 to not close the socket.
    void postAccept(int fd);
    int preClose(int fd); // return -1 to not close the socket.
    void postClose(int fd);
private:
    Incoming *i;
    Queue *q;
};

#endif /* ifndef NETHANDLE_H_INCLUDED */
