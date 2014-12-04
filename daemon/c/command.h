#ifndef COMMAND_H_INCLUDED
#define COMMAND_H_INCLUDED

//#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>

#ifndef DEBUG
    #define DEBUG 8
#endif

#ifndef DBGTHREAD
    #define DBGTHREAD 8
#endif

struct Mutex {
    pthread_mutex_t mutex;
    const char *name;
};

typedef struct Mutex Mutex;

#define COMMAND_UNKNOWN 0
#define COMMAND_ECHO    1
#define COMMAND_INFO    2

// state:
//  1- not started.
//  2- started
//  4- processed
//  8- finished
//
struct Command {
    char            *command;
    char            *reply;
    unsigned int     command_number;
    unsigned int     fdsindex;
    struct Command  *next;
    struct Command **last;
    unsigned int     state;
    pthread_mutex_t  lock;
    pthread_cond_t   cond;
};

struct QueueNode {
    void *nodeData;
    struct QueueNode  *next;
    struct QueueNode  *prev;
    struct QueueNode **last;
    Mutex             *mutex;
};

typedef struct Command Command;
typedef struct QueueNode QueueNode;

extern Mutex *q_mutex;
extern QueueNode **qLast;
extern QueueNode **qFirst;
//extern pthread_mutex_t q_mutex;

void qinit();
void qdestroy();

QueueNode *qninit(void *data);
void qndestroy(QueueNode *n);

Command **last;
Command **first;
extern pthread_mutex_t cqlast_mutex;
extern pthread_mutex_t cqfirst_mutex;

void *thread_processCommand(void *data);

void cqueueInit();
Command *initCommand();
void freeCommand(Command *c);
void processCommand();

int mutex_lock(Mutex *m);
int mutex_trylock(Mutex *m);
int mutex_unlock(Mutex *m);
Mutex *mutex_create(const char *name);
void mutex_init(Mutex *m, const char *name);
void mutex_delete(Mutex *m);

inline void debug(int level, const char *fmt, ...);
inline void vdebug(int level, const char *fmt, va_list l);
inline void tdebug(int level, const char *fmt, ...);
inline void vtdebug(int level, const char *fmt, va_list l);

#endif // ifndef COMMAND_H_INCLUDED
