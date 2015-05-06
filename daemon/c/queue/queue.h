#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include "nodebase.h"

class Queue {
public:
    Queue();
    ~Queue();

    // lock();
    // unlock();
    // trylock();

    void queue(NodeBase *n);
    NodeBase *dequeue();

private:
    NodeBase *head;
    NodeBase *tail;
};

#endif /* ifndef QUEUE_H_INCLUDED */
