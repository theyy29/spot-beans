#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

#include <thread>

class Thread {
public:
    Thread();
    ~Thread();

    int start();
    int run();

    int join();
private:
    std::thread *thread;
};

#endif /* ifndef THREAD_H_INCLUDED */
