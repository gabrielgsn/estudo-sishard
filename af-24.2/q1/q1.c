// Leia o arquivo README.md ou README.html antes de iniciar este exercício!
// para compilar use:
// gcc q1.c -o q1 -pthread

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

void *thread1(void *_arg)
{

    printf("A\n");

    printf("F\n");

    return NULL;
}

void *thread2(void *_arg)
{

    printf("C\n");

    return NULL;
}

void *thread3(void *_arg)
{

    printf("B\n");

    printf("D\n");

    return NULL;
}

void *thread4(void *_arg)
{

    printf("E\n");

    printf("G\n");

    return NULL;
}

int main(int argc, char *argv[])
{

    // Crie TODAS as threads. Voce deve utilizar semaforos para sincronizacao.

    // Espere por TODAS as threads

    return 0;
}