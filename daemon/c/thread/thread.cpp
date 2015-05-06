#include "thread.h"


Thread::Thread(){
}

Thread::~Thread(){
}

int Thread::start(){
    //thread = new std::thread(run);
}

int Thread::run(){
    printf("Thread\n");
}

int Thread::join(){
    thread->join();
}
