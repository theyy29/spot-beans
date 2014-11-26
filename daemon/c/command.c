#include "command.h"
#include <stdio.h>
#include <string.h>

pthread_mutex_t cqlast_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t cqfirst_mutex = PTHREAD_MUTEX_INITIALIZER;

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

void debug(int level, const char *fmt, ...){
    va_list l;
    va_start(l, fmt);
    vdebug(level, fmt, l);
    va_end(l);
}
void vdebug(int level, const char *fmt, va_list l){
    if(DEBUG <= level)
        vprintf(fmt, l);
}

void *thread_processCommand(void *data){
    Command *d = *(Command **)data;
    if(pthread_mutex_trylock(&(d->lock)) != 0){
        // the command is in use.
        // there is no point to try to wait to process it. return.
        return;
    }
    Command *p = d;
    p->reply = (char *)malloc(strlen(p->command) + 1);
    memcpy(p->reply, p->command, strlen(p->command) + 1);

    pthread_t thread_id = pthread_self();

    printf("Handling Command In THREAD(%lu):\n", thread_id);
    printf("\tCommand: %s\n", p->command);
    printf("\tReply:   %s\n", p->reply);
    printf("\tfds[%d]\n", p->fdsindex);
    printf("\tnext command: %lu\n", (unsigned long)(p->next));
    printf("Finished command.\n");
    p->state = 4;
    pthread_mutex_unlock(&(d->lock));
    //pthread_cond_signal(&(d->cond));
}
