#include "queue.h"

#include <stdlib.h>

Queue::Queue(){
    head = NULL;
    tail = NULL;
}

Queue::~Queue(){
    // If there are any commands, print out info about them being skipped.
}

void Queue::queue(NodeBase *n){
    if(tail){
        tail->setNext(n);
        n->setPrev(tail);
        tail = n;
    } else {
        head = tail = n;
    }
}

NodeBase *Queue::dequeue(){
    if(head){
        NodeBase *h = head;
        NodeBase *nh = h->next();
        if(nh)
            (head = nh)->setPrev(NULL);
        else
            head = tail = NULL;
        return h;
    }
    return NULL;
}
/*

Queue::lock(){
}

Queue::unlock(){
}

Queue::trylock(){
}

*/
