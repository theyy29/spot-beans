#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

//#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef DEBUG
    #define DEBUG 8
#endif

// state:
//  1- not started.
//  2- started
//  4- processed
//  8- finished
//
struct Command {
    char            *command;
    char            *reply;
    unsigned int     fdsindex;
    struct Command  *next;
    struct Command **last;
    unsigned int     state;
    pthread_mutex_t  lock;
    pthread_cond_t   cond;
};

typedef struct Command Command;

Command **last;
Command **first;
extern pthread_mutex_t cqlast_mutex;
extern pthread_mutex_t cqfirst_mutex;

void cqueueInit();
Command *initCommand();
void processCommand();

void debug(int level, const char *fmt, ...);
void vdebug(int level, const char *fmt, va_list l);

#endif // ifndef COMMAND_H_INCLUDED
