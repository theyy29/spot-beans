#include <iostream>

#include "net.h"
#include "nethandle.h"
#include "queue/queue.h"
#include "queue/command.h"

#define PORT "9034" // port we're listening on
#define null ((void *)0)
#define MAXFDCOUNT 100

int main(int argc, char *argv[]){
    Queue *q;
    NetHandle *n;

    q = new Queue();
    n = new NetHandle(q);

    //Incoming i;
    //i.bindForListen();
    while(true){
        n->poll();
        Command *c = (Command *)q->dequeue();
        if(c){
            std::cout << c->DBGECHO();
            std::cout << "handling command\n";
        }
        //i.poll();
    }
}
