#include "command.h"
#include <stdio.h>
#include <string.h>

//Command **last;
//Command **first;

// SETUP FOR CONTROL
// Command **last  = malloc(sizeof(Command *));
// Command **first = malloc(sizeof(Command *));
// *last  = NULL;
// *first = NULL;

//while (*first && (*first)->state == 4){
//int r = send(fds[(*first)->fdsindex].fd, (*first)->reply, strlen((*first)->reply), 0);
// Finish with the command
//Command *c = *first;
//*first = c->next;
//if(*last == c){
//    *last = NULL;
//}
//debug(0, "Freeing c->command: %lu\n", (long unsigned int)c->command);
//debug(0, "Freeing c->reply:   %lu\n", (long unsigned int)c->reply);
//debug(0, "Freeing c:          %lu\n", (long unsigned int)c);
//free(c->command);
//free(c->reply);
//free(c);
//// That command is done.
//
//Command *c = initCommand();
//c->fdsindex = i;
//c->command = buf;
//c->command[nread - 1] = '\0'; // This will work, the buffer is at least one byte bigger than the cuount asked for
//if(*first == NULL){
//    *first = c;
//    *last  = c;
//} else {
//    // append it to the last one, and then say the last is it
//    (*last)->next = c;
//    *last = c;
//}

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
    last = malloc(sizeof(Command *));
    first = malloc(sizeof(Command *));
    *last = NULL;
    *first = NULL;
}

Command *initCommand(){
    Command *c = (Command *)malloc(sizeof(Command));
    memset(c, 0, sizeof(Command));
    c->last = last;
    c->next = NULL;
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

