// Leia o arquivo README.md ou README.html antes de iniciar este exercício!
// para compilar use:

// gcc q1.c -o q1 -pthread

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {

} thread_arg;

void *thread1(void *_arg) {

    printf("A\n");

    printf("C\n");

    printf("E\n");

    return NULL;
}

void *thread2(void *_arg) {

    printf("F\n");

    return NULL;
}

void *thread3(void *_arg) {

    printf("D\n");

    return NULL;
}

void *thread4(void *_arg) {


    printf("B\n");

    printf("G\n");

    return NULL;
}

int main(int argc, char *argv[]) {

    return 0;
}