#include "command.h"
#include "commands.h"

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

pthread_mutex_t cqlast_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cqfirst_mutex = PTHREAD_MUTEX_INITIALIZER;

QueueNode **qLast;
QueueNode **qFirst;
Mutex      *q_mutex;
// pthread_mutex_t q_mutex = PTHREAD_MUTEX_INITIALIZER;

void qinit(){
    qLast   = (QueueNode **)malloc(sizeof(QueueNode *));
    qFirst  = (QueueNode **)malloc(sizeof(QueueNode *));
    //q_mutex = (Mutex *)malloc(sizeof(Mutex));

    *qLast  = NULL;
    *qFirst = NULL;
    q_mutex = mutex_create("General queue mutex lock");
}

void qdestroy(){
    mutex_lock(q_mutex);
    if(*qLast){
        debug(8, "Error: trying to free qLast when *qLast is not NULL\n");
        debug(8, "\t(*qLast)->nodeData: %lu\n",
                (unsigned long int) (*qLast)->nodeData);
        debug(8, "\t(*qLast)->next:     %lu\n",
                (unsigned long int) (*qLast)->next);
        debug(8, "\t(*qLast)->last:     %lu\n",
                (unsigned long int) (*qLast)->last);
        if((*qLast)->last)
            debug(8, "\t*((*qLast)->last):  %lu\n",
                    (unsigned long int) *((*qLast)->last));
    }
    if(*qFirst){
        debug(8, "Error: trying to free qLast when *qLast is not NULL\n");
        debug(8, "\t(*qFirst)->nodeData: %lu\n",
                (unsigned long int) (*qFirst)->nodeData);
        debug(8, "\t(*qFirst)->next:     %lu\n",
                (unsigned long int) (*qFirst)->next);
        debug(8, "\t(*qFirst)->last:     %lu\n",
                (unsigned long int) (*qFirst)->last);
        if((*qFirst)->last)
            debug(8, "\t*((*qFirst)->last):  %lu\n",
                    (unsigned long int) *((*qFirst)->last));
    }
    free(qLast);
    free(qFirst);
    mutex_unlock(q_mutex);
    mutex_delete(q_mutex);
}

QueueNode *qninit(void *data){
    printf("SIZE OF QueueNode=%lu\n", sizeof(QueueNode));
    printf("SIZE OF     Mutex=%lu\n", sizeof(Mutex));
    QueueNode *n = (QueueNode *)malloc(sizeof(QueueNode));
    memset(n, 0, sizeof(QueueNode));
    n->nodeData = data;
    n->last = qLast;
    n->mutex = mutex_create("Command mutex");
    //mutex_init(n->mutex, "Command mutex");
    return n;
}
// The n->nodeData should have already been deleted, or be about to be deleted
void qndestroy(QueueNode *n){
    pthread_mutex_destroy(&(n->mutex->mutex));
    free(n);
}

Mutex *mutex_create(const char *name){
    Mutex *m = (Mutex *)malloc(sizeof(Mutex));
    mutex_init(m, name);
    return m;
}

void mutex_init(Mutex *m, const char *name){
    tdebug(2, "MUTEX CREATION FOR \"%s\"\n", name);
    m->name = name;
    pthread_mutex_init(&(m->mutex), NULL);
}

void mutex_delete(Mutex *m){
    pthread_mutex_destroy(&(m->mutex));
    free(m);
}

void qadd(void *node){
    mutex_lock(q_mutex);
    QueueNode *last;
    mutex_unlock(q_mutex);
}

void processCommand(){
    if(!*first) return;
    Command *p = *first;
    while(p->state == 4){
        p = p->next;
    }
    p->reply = (char *)malloc(strlen(p->command) + 1);
    memcpy(p->reply, p->command, strlen(p->command) + 1);
    printf("Handling Command:\n");
    printf("\tCommand: %s\n", p->command);
    printf("\tReply:   %s\n", p->reply);
    printf("\tfds[%d]\n", p->fdsindex);
    printf("\tnext command: %lu\n", (unsigned long)(p->next));
    printf("Finished command.\n");
    p->state = 4;
}

void cqueueInit(){
    if(pthread_mutex_trylock(&cqlast_mutex)){
        printf("ERROR LOCKING FOR Command **last IN cqueueInit(): Mutex already locked.\nThis should NEVER happen. cqueueInit() MUST be called BEFORE ANYTHING ELSE\n");
        pthread_mutex_lock(&cqlast_mutex);
    }
    last = malloc(sizeof(Command *));
    *last = NULL;
    pthread_mutex_unlock(&cqlast_mutex);

    if(pthread_mutex_trylock(&cqfirst_mutex)){
        printf("ERROR LOCKING FOR Command **first IN cqueueInit(): Mutex already locked.\nThis should NEVER happen. cqueueInit() MUST be called BEFORE ANYTHING ELSE\n");
        pthread_mutex_lock(&cqfirst_mutex);
    }
    first = malloc(sizeof(Command *));
    *first = NULL;
    pthread_mutex_unlock(&cqfirst_mutex);
}

Command *initCommand(){
    Command *c = (Command *)malloc(sizeof(Command));
    memset(c, 0, sizeof(Command));
    c->last = last;
    c->next = NULL;
    pthread_mutex_init(&(c->lock), NULL);
    pthread_cond_init(&(c->cond), NULL);
    return c;
}

void freeCommand(Command *c){
    free(c->command);
    free(c->reply);
    pthread_mutex_destroy(&(c->lock));
    pthread_cond_destroy(&(c->cond));
    free(c);
}

int mutex_lock(Mutex *m){
    struct timeval *b, *a;
    b = (struct timeval *)malloc(sizeof(struct timeval));
    a = (struct timeval *)malloc(sizeof(struct timeval));

    gettimeofday(b, NULL);
    tdebug(3, "LOCKing \"%s\" at %d.%d\n", m->name, b->tv_sec, b->tv_usec);

    int r = pthread_mutex_lock(&(m->mutex));

    gettimeofday(a, NULL);
    if(r != 0)
        tdebug(3, "LOCK    \"%s\" FAILED at %d.%d\n", m->name, a->tv_sec, a->tv_usec);
    else
        tdebug(3, "LOCKed  \"%s\" at %d.%d\n", m->name, a->tv_sec, a->tv_usec);

    free(a); free(b);

    return r;
}

int mutex_trylock(Mutex *m){
    struct timeval *b;
    struct timeval *a;
    b = (struct timeval *)malloc(sizeof(struct timeval));
    a = (struct timeval *)malloc(sizeof(struct timeval));

    gettimeofday(b, NULL);
    tdebug(3, "TRY LOCKing \"%s\" at %d.%d\n", m->name, b->tv_sec, b->tv_usec);

    int r = pthread_mutex_trylock(&(m->mutex));

    gettimeofday(a, NULL);
    if(r != 0)
        tdebug(3, "TRY LOCK    \"%s\" FAILED at %d.%d\n", m->name, a->tv_sec, a->tv_usec);
    else
        tdebug(3, "TRY LOCKed  \"%s\" at %d.%d\n", m->name, a->tv_sec, a->tv_usec);

    free(a); free(b);

    return r;
}

int mutex_unlock(Mutex *m){
    struct timeval *b, *a;
    b = (struct timeval *)malloc(sizeof(struct timeval));
    a = (struct timeval *)malloc(sizeof(struct timeval));

    gettimeofday(b, NULL);
    tdebug(3, "UNLOCKing \"%s\" at %d.%d\n", m->name, b->tv_sec, b->tv_usec);

    int r = pthread_mutex_unlock(&(m->mutex));

    gettimeofday(a, NULL);
    if(r != 0)
        tdebug(3, "UNLOCK    \"%s\" FAILED at %d.%d\n", m->name, a->tv_sec, a->tv_usec);
    else
        tdebug(3, "UNLOCKed  \"%s\" at %d.%d\n", m->name, a->tv_sec, a->tv_usec);

    free(a); free(b);

    return r;
}

inline void debug(int level, const char *fmt, ...){
    va_list l;
    va_start(l, fmt);
    vdebug(level, fmt, l);
    va_end(l);
}
inline void vdebug(int level, const char *fmt, va_list l){
    if(DEBUG <= level)
        vprintf(fmt, l);
}

inline void tdebug(int level, const char *fmt, ...){
    va_list l;
    va_start(l, fmt);
    vtdebug(level, fmt, l);
    va_end(l);
}
inline void vtdebug(int level, const char *fmt, va_list l){
    if(DBGTHREAD <= level)
        vprintf(fmt, l);
}

void *thread_processCommand(void *data){
    printf("THREAD CREATED\n");
    fflush(stdout);
    //Command *d = *(Command **)data;
    QueueNode *qn = (QueueNode *)data;
    Command *d = (Command *)(qn->nodeData);
    if(mutex_trylock(qn->mutex)){
        debug(5, "Command mutex already locked when thread tried to access it\n");
        // the command is in use.
        // there is no point to try to wait to process it. return.
        return;
    }
    Command *p = d;
    // p->reply = (char *)malloc(strlen(p->command) + 1);
    // memcpy(p->reply, p->command, strlen(p->command) + 1);
    c_echo(p);

    pthread_t thread_id = pthread_self();

    printf("Handling Command In THREAD(%lu):\n", thread_id);
    printf("\tCommand: %s\n", p->command);
    printf("\tReply:   %s\n", p->reply);
    printf("\tfds[%d]\n", p->fdsindex);
    printf("\tnext command: %lu\n", (unsigned long)(p->next));
    printf("Finished command.\n");
    p->state = 4;
    mutex_unlock(qn->mutex);
}

void parseCommand(Command *c){
}
