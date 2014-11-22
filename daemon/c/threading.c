#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "command.h"

void *thread1f(void *arg){
    printf("Greetings from thread 1.\n");
    int *p = malloc(sizeof(int));
    *p = 0;
    return p;
}

void *thread2f(void *arg){
    printf("Greetings from thread 2.\n");
    int *p = malloc(sizeof(int));
    *p = 5;
    return p;
}

int main(int argc, char *argv[]){
    printf("THREADING\n");

    cqueueInit();

    pthread_t thread1, thread2;

    Command *c1, *c2;
    c1 = initCommand();
    c2 = initCommand();
    
    int r1 = 0, r2 = 0;
    r1 = pthread_create(&thread1, NULL, thread1f, (void *)c1);
    if(r1 != 0){ printf("Error making thread1\n"); }
    r2 = pthread_create(&thread2, NULL, thread2f, (void *)c2);
    if(r2 != 0){ printf("Error making thread2\n"); }

    int **t1, **t2;
    t1 = malloc(sizeof(int));
    t2 = malloc(sizeof(int));

    int d1 = 0, d2 = 0;
    d1 = pthread_join(thread1, (void **)t1);
    d2 = pthread_join(thread2, (void **)t2);

    printf("Threads done with values %d, %d\n", **t1, **t2);

    return 0;
}

