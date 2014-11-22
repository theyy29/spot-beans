#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>
#include <stdarg.h>

#define PORT "9034" // port we're listening on
#define null ((void *)0)
#define MAXFDCOUNT 100

// state:
//  1- not started.
//  2- started
//  4- processed
//  8- finished
//
struct Command {
    char *command;
    char *reply;
    unsigned int fdsindex;
    struct Command *next;
    struct Command **last;
    unsigned int state;
};
typedef struct Command Command;

Command **last;
Command **first;


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int acceptconnection(struct pollfd *fds, unsigned int acceptfrom);
void endconnection(struct pollfd *pfdp);
Command *initCommand();
void processCommand();

void debug(int level, const char *fmt, ...);
void vdebug(int level, const char *fmt, va_list l);


int main(int argc, char *argv[])
{
    int fdmax; // maximum file descriptor number
    int listener; // listening socket descriptor
    char remoteIP[INET6_ADDRSTRLEN];
    int yes=1; // for setsockopt() SO_REUSEADDR, below
    int i, j, rv;
    struct addrinfo hints, *ai, *p;
    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }
        // lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }
         break;
    }
    // if we got here, it means we didn't get bound
    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }
    freeaddrinfo(ai); // all done with this
    // listen
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // SETUP FOR CONTROL
    //Command **last  = malloc(sizeof(Command *));
    //Command **first = malloc(sizeof(Command *));
    last  = malloc(sizeof(Command *));
    first = malloc(sizeof(Command *));
    *last  = null;
    *first = null;

    // SETUP FOR MAIN LOOP
    printf("Allocating %lu bytes for the struct pollfd array\n", sizeof(struct pollfd) * 100);
    // Create:
    int fdsn = 1;
    struct pollfd *fds;
    fds = (struct pollfd *)malloc(sizeof(struct pollfd) * MAXFDCOUNT);
    memset(fds, 0, sizeof(struct pollfd) * MAXFDCOUNT);
    ///fds[0] = (struct pollfd)malloc(sizeof(fds));
    fds[0].fd = listener;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    // fds array
    // length of array
    // timespec *time
    // sigset_t *sigmask

    struct timespec *time = malloc(sizeof(struct timespec));
    time->tv_sec = 0;
    time->tv_nsec = 1000;

    // main loop
    // int ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout_ts, const sigset_t *sigmask);
    //ppoll(null, 0, null, null);
    //ppoll(fds, 1, null, null);
    printf("THING %d\n", DEBUG);
    printf("Starting main loop with listener\'s fd = %d\n", fds[0].fd);
    while(1){
        int timeout = 10; // a second
        if(*first == null){
            timeout = -1; // wait fo eva
        }
        int val = poll(fds, fdsn, timeout);
        if(!val){
            // This will break if we do audio in this thread
            while (*first && (*first)->state == 4){
                //send(fds[(*first)->fdsindex].fd, (*first)->reply, strlen((*first)->reply), MSG_DONTWAIT);
                int r = send(fds[(*first)->fdsindex].fd, (*first)->reply, strlen((*first)->reply), 0);
                if(r == -1){
                    //something went wrong.
                    printf("Error writing response to socket: %d\n", fds[(*first)->fdsindex].fd);
                    printf("\tCommand: %s\n\tReply: %s\n", (*first)->command, (*first)->reply);
                    break;
                } else {
                    Command *c = *first;
                    *first = c->next;
                    if(*last == c){
                        *last = null;
                    }
                    debug(0, "Freeing c->command: %lu\n", (long unsigned int)c->command);
                    debug(0, "Freeing c->reply:   %lu\n", (long unsigned int)c->reply);
                    debug(0, "Freeing c:          %lu\n", (long unsigned int)c);
                    free(c->command);
                    free(c->reply);
                    // free(c->last); // unneeded. this (the pointer, not the thing getting pointed to from the pointer) is allocated when the struct is
                    free(c);
                    // That command is done.
                }
            }
            processCommand();
        }
//      if(!val){
//          putchar('.');
//          fflush(stdout);
//          continue;
//      }
        debug(3, "\nCaught %d event(s) that need to be done\n", val);
        int i = 0;
        for(i = 0; val > 0 && i < fdsn ; ++i){
            // POLLIN POLLOUT POLLERR POLLHUP
            if(fds[i].revents & POLLIN){
                // either read if its a accepted socket, or accept if its the listening socket.
                if(i == 0){ // aka it is the listening socket
                    if(fdsn == MAXFDCOUNT){
                        printf("Cannot accept socket: maximum sockets already open\n");
                    } else {
                        // Accept the connection
                        int r = acceptconnection(fds, i);
                        if(r > 0) ++fdsn;
                        debug(1, "\tThe new size of fds is %d\n", fdsn);
                    }
                } else {
                    int len = 2047;
                    char *buf = (char *)malloc(len+1);
                    int nread = recv(fds[i].fd, (void *)buf, len, MSG_DONTWAIT);
                    if(nread == 0){
                        printf("Hanging up on fd %d\n", fds[i].fd);
                        endconnection(fds+i);
                        --fdsn;
                        free(buf);
                    } else {
                        debug(3, "READ FROM SOCKET: %d:\n\tAsked for %d bytes, was given %d. Message was:\n%s\n\t", fds[i].fd, len, nread, buf);
                        debug(1, "\tbuf is %x\n", buf);

                        Command *c = initCommand();
                        c->fdsindex = i;
                        c->command = buf;
                        c->command[nread - 1] = '\0'; // This will work, the buffer is at least one byte bigger than the cuount asked for
                        if(*first == null){
                            *first = c;
                            *last  = c;
                        } else {
                            // append it to the last one, and then say the last is it
                            (*last)->next = c;
                            *last = c;
                        }
                    }
                }
            }
            if(fds[i].revents & POLLHUP){
                printf("Closing connection on fd %d\n", fds[i].fd);
                close(fds[i].fd);
                memset(fds+i, 0, sizeof(struct pollfd));
                --fdsn;
            }
        }
        // val < 0 -> error
        // val = 0 -> timeout
        // val > 0 = number of sockets with changes
    }
    return 0;
}

int acceptconnection(struct pollfd *fds, unsigned int acceptfrom){
    // find the next available element in the array to put the connection into
    // Start at one, which is after the socket we are accepting from.
    int nindex = 1;
    for(; nindex < MAXFDCOUNT; ++nindex){
        if(fds[nindex].fd <= 0){ // This will break if the stdin file is trying to be read from
            break;
        }
    }
    struct sockaddr *s = (struct sockaddr *)malloc(sizeof(struct sockaddr));
    socklen_t len = 0;
    fds[nindex].fd = accept(fds[0].fd, s, &len);
    debug(8, "NEW SOCKET ACCEPTED: fd is %d\n", fds[nindex].fd);
    free(s);
    if(fds[nindex].fd > 0){
        fds[nindex].events = POLLIN | POLLHUP;
        debug(2, "\tfds[%d].fd = %d\n", nindex, fds[nindex].fd);
        return fds[nindex].fd;
    }
    fds[nindex].fd = 0;
    return -1;
}

void endconnection(struct pollfd *pfdp){
    close(pfdp->fd);
    memset((void *)pfdp, 0, sizeof(struct pollfd));
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

Command *initCommand(){
    Command *c = (Command *)malloc(sizeof(Command));
    memset(c, 0, sizeof(Command));
    c->last = last;
    c->next = null;
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

