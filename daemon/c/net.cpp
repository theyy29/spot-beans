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

#include "queue/command.h"

#define PORT "9034" // port we're listening on
#define MAXFDCOUNT 100

#include "net.h"

Incoming::Incoming(){
    bound = false;

    fdz = NULL;

    cbs = NULL;

    listener = 0;

    fdcount = 0;
}

Incoming::~Incoming(){
    if(bound){
        // close connection...?
    }
}

// Bind to a socket. for future, maybe specify force ipv4/ipv6 or preference.
int Incoming::bindForListen(){
    if(bound) return -1;

    int err;

    struct addrinfo hints, *ai, *p;
    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET AF_INET6
    hints.ai_socktype = SOCK_STREAM; // SOCK_DGRAM or 0
    hints.ai_flags = AI_PASSIVE;

    if ((err = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(err));
        return -1;
    }

    // Bind to the first one we can
    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }
        int yes = 0;
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
        fprintf(stderr, "Incoming::bindForListen: failed to bind\n");
        return -2;
    }

    freeaddrinfo(ai); // all done with this

    // listen, allowing up to 10 pending connection attempts
    if (listen(listener, 10) == -1) {
        perror("listen");
        return -3;
    }

    bound = true;

    addDescriptor(listener, POLLIN);

    return 0;
}

int Incoming::closeAll(){
    // First things first, unbind the listen socket.
    close(listener);
    removeDescriptor(listener);
    listener = 0;

    if(fdz){
        for(int i = 0; i < fdcount; ++i){
            close(fdz[i].fd);
        }

        free(fdz);
        fdz = NULL;
        fdcount = 0;
    }

    bound = false;
    return 0;
}

int Incoming::poll(){
    // fds array
    // length of array
    // timespec *time
    // sigset_t *sigmask

    if(fdcount == 0) return 0;

    int timeout = 100;

    int val = ::poll(fdz, fdcount, timeout);
    if(!val);
    for(int i = 0; val > 0 && i < fdcount ; ++i){
        // POLLIN POLLOUT POLLERR POLLHUP
        if(fdz[i].revents & POLLHUP){
            int v = 0;
            v = cbs->preClose(fdz[i].fd);
            if(v < 0){
                printf("IGNORING SOCKET POLLHUP\n");
            } else {
                close(fdz[i].fd);
                removeDescriptor(fdz[i].fd);
                cbs->postClose(fdz[i].fd);
            }
        } else {
            if(fdz[i].revents & POLLIN){
                // either read if its a accepted socket, or accept if its the listening socket.
                if(fdz[i].fd == listener){
                    // Accept the socket
                    int v = 0;
                    v = cbs->preAccept(fdz[i].fd);
                    if(v < 0){ // dont accept
                        printf("IGNORING SOCKET ACCEPT REQUEST!\n");
                    } else {
                        struct sockaddr *s = (struct sockaddr *)malloc(sizeof(struct sockaddr));
                        socklen_t len = 0;
                        int fd = accept(fdz[0].fd, s, &len);
                        //debug(8, "NEW SOCKET ACCEPTED: fd is %d\n", fd);
                        addDescriptor(fd);
                        free(s);
                        cbs->postAccept(fd);
                    }
                } else {
                    int nread = 0;
                    nread = cbs->read(fdz[i].fd);
                    if(nread == 0){
                        int v = 0;
                        v = cbs->preClose(fdz[i].fd);
                        if(v < 0){
                            printf("IGNORING SOCKET EOS\n");
                        } else {
                            close(fdz[i].fd);
                            removeDescriptor(fdz[i].fd);
                            cbs->postClose(fdz[i].fd);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void Incoming::setCallbacks(IncomingCallbacks *cb){
    cbs = cb;
//    printf("Setting callbacks...\n");
//    printf("read       = %lx\n", (unsigned long int)cbs->read);
//    printf("preAccept  = %lx\n", (unsigned long int)cbs->preAccept);
//    printf("postAccept = %lx\n", (unsigned long int)cbs->postAccept);
//    printf("preClose   = %lx\n", (unsigned long int)cbs->preClose);
//    printf("postClose  = %lx\n", (unsigned long int)cbs->postClose);
}

void Incoming::addDescriptor(int fd, int events){
    ++fdcount;
    printf("ADDing descriptor. (%d). Total: %d\n", fd, fdcount);

    if(!fdz){
        fdz = (pollfd *)malloc(sizeof(pollfd));
        memset(fdz, 0, sizeof(pollfd));
        fdz[0].fd = fd;
        fdz[0].events = events;
        return;
    }

    printf("realloc() %lu -> %lu\n", sizeof(pollfd) * (fdcount-1), sizeof(pollfd) * fdcount);
    fdz = (pollfd *)realloc(fdz, sizeof(pollfd) * fdcount); // stretch the memory // <--
    memset(fdz + (fdcount-1), 0, sizeof(pollfd)); // clear the new one

    fdz[fdcount-1].fd = fd;
    fdz[fdcount-1].events = events;
}

void Incoming::removeDescriptor(int fd){
    if(!fdcount) return;

    // find the block with our fd.
    int i = 0;
    for(; i < fdcount; ++i)
        if(fdz[i].fd == fd) break;

    if(i == fdcount - 1){
        printf("Chopping off the last block\n");
        if(fdcount - 1 == 0){
            free(fdz);
            fdz = NULL;
            fdcount = 0;
            return;
        }
    } else {
        printf("Shifting blocks...\n");
        printf("blocks (%d -> %d) -> (%d -> %d)\n", i+1, fdcount-1, i, fdcount-2);

        memmove(fdz+i, fdz+i+1, sizeof(pollfd) * (fdcount - i - 1));
    }

    fdz = (pollfd *)realloc(fdz, sizeof(pollfd) * fdcount - 1);

    --fdcount;

    return;


/*
    if(!fdcount) return;
    printf("REMOVE descriptor. (%d). Total: %d\n", fd, fdcount - 1);

    int i = 0;
    for(; i < fdcount; ++i){
        printf("fdz[%d].fd == %d\n", i, fdz[i].fd);
        if(fdz[i].fd == fd) break;
    }

    //if(i == 0) { // It shouldnt ever be zero unless we close the listening socket.
    if(i != fdcount - 1) {
        memmove((void *)(fdz + i + 1), (const void *)(fdz + i), sizeof(pollfd) * (fdcount - (i-1))); // <--
    } else {
        //memmove((void *)(fdz + 1), (const void *)fdz, sizeof(pollfd) * (fdcount-2)); // 1 indexed + offset 1 = 2
    }

    printf("i == %d; moving block %d and on to %d (%d bytes)\n", i, i+1, i, sizeof(pollfd) * (fdcount - (i-1)));
    printf("new size: %d\n", fdcount-1);
    fdz = (pollfd *)realloc(fdz, sizeof(pollfd) * (fdcount-1));
    --fdcount;
    */
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addri2(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/*

int acceptconnection2(struct pollfd *fds, unsigned int acceptfrom);
void endconnection2(struct pollfd *pfdp);

int main2(int argc, char *argv[]){
    printf("THREADING WITH SOCKETS AND SPOTIFY.\n");
    int fdmax; // maximum file descriptor number
    int listener; // listening socket descriptor
    char remoteIP[INET6_ADDRSTRLEN];
    int yes=1; // for setsockopt() SO_REUSEADDR, below
    int i, j, rv;
    struct addrinfo hints, *ai, *p;
    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET AF_INET6
    hints.ai_socktype = SOCK_STREAM; // SOCK_DGRAM or 0
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
    cqueueInit();
    qinit();

    pthread_t thread;

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


    // Set up the spotify connection
    ////////////////////////////////

    // Create a session config thingy
    
    struct sp_session_config *config = spi_init_session_config("spkey"); 

    // Create a session
    sp_session_create(config, NULL);

    // main loop
    // int ppoll(struct pollfd *fds, nfds_t nfds, const struct timespec *timeout_ts, const sigset_t *sigmask);
    //ppoll(null, 0, null, null);
    //ppoll(fds, 1, null, null);
    printf("THING %d\n", DEBUG);
    printf("Starting main loop with listener\'s fd = %d\n", fds[0].fd);
    while(1){
        int timeout = 10; // a second
        //if(*first == null || *q){
        if(*qFirst == NULL){
            timeout = -1; // wait fo eva
        }
        int val = poll(fds, fdsn, timeout);
        if(!val){
            int r = mutex_trylock(q_mutex);
            if(r == 0){
                QueueNode *n = *qFirst;
                while(n){
                        if(mutex_trylock(n->mutex) == 0){
                        Command *node = (Command *)(n->nodeData);
                        if(node->state == 0){
                            // Pass off the mutex to the thread, and go to the next iteration
                            mutex_unlock(n->mutex);
                            pthread_create(&thread, NULL, thread_processCommand, n);
                            n = n->next;
                            continue;
                        } else if(node->state == 4){
                            int result = send(fds[node->fdsindex].fd, node->reply, strlen(node->reply), 0);
                            if(result != -1){
                                node->state = 8;
                            }
                            mutex_unlock(n->mutex);
                            n = n->next; // continue;
                            continue;
                        } else if (node->state == 8){
                            QueueNode *tis = n;
                            // it needs to be deleted.
                            if(n == *qFirst)
                                *qFirst = n->next;
                            if(n == *qLast)
                                *qLast = n->prev;
                            n = n->next;
                            freeCommand(tis->nodeData);
                            mutex_unlock(tis->mutex);
                            qndestroy(tis);
                            continue;
                        }
                        mutex_unlock(n->mutex);
                    }
                    n = n->next;
                }
            }
            mutex_unlock(q_mutex);
            /*
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
            //processCommand();
            * /
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
                        QueueNode *n = qninit(c);
                        //n->last = qLast;
                        n->prev = *qLast;
                        *qLast = n;
                        if(!*qFirst){
                            *qFirst = *qLast;
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


    qdestroy();
    return 0;
}

int acceptconnection2(struct pollfd *fds, unsigned int acceptfrom){
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

void endconnection2(struct pollfd *pfdp){
    close(pfdp->fd);
    memset((void *)pfdp, 0, sizeof(struct pollfd));
}
*/
