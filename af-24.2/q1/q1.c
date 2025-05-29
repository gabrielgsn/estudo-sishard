// Leia o arquivo README.md ou README.html antes de iniciar este exercício!
// para compilar use:
// gcc q1.c -o q1 -pthread

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

struct args{
    sem_t s1, s2, s3, s4;
};

void *thread1(void *_arg)
{
    struct args *sems = (struct args *)_arg;

    printf("A\n");

    sem_post(&sems->s1);
    sem_post(&sems->s1);
    sem_post(&sems->s1);

    sem_wait(&sems->s2);
    sem_wait(&sems->s3);
    sem_wait(&sems->s4);


    printf("F\n");

    return NULL;
}

void *thread2(void *_arg)
{
    struct args *sems = (struct args *)_arg;

    sem_wait(&sems->s1);
    sem_wait(&sems->s3);

    printf("C\n");

    sem_post(&sems->s2);
    sem_post(&sems->s2);

    return NULL;
}

void *thread3(void *_arg)
{
    struct args *sems = (struct args *)_arg;

    printf("B\n");

    sem_post(&sems->s3);
    sem_post(&sems->s3);

    sem_wait(&sems->s1);

    printf("D\n");

    sem_post(&sems->s3);
    sem_post(&sems->s3);

    return NULL;
}

void *thread4(void *_arg)
{
    struct args *sems = (struct args *)_arg;

    sem_wait(&sems->s1);
    sem_wait(&sems->s3);

    printf("E\n");

    sem_post(&sems->s4);
    sem_wait(&sems->s2);
    sem_wait(&sems->s3);

    printf("G\n");

    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t t1, t2, t3, t4;
    struct args sems;
    sem_init(&sems.s1, 0, 0);
    sem_init(&sems.s2, 0, 0);
    sem_init(&sems.s3, 0, 0);
    sem_init(&sems.s4, 0, 0);

    // Crie TODAS as threads. Voce deve utilizar semaforos para sincronizacao.

        // Inicialize os semaforos e mutexes
    pthread_create(&t1, NULL, thread1, (void *)&sems);
    pthread_create(&t2, NULL, thread2, (void *)&sems);
    pthread_create(&t3, NULL, thread3, (void *)&sems);
    pthread_create(&t4, NULL, thread4, (void *)&sems);


    // Espere por TODAS as threads
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    // Destrua os semaforos e mutexes
    sem_destroy(&sems.s1);
    sem_destroy(&sems.s2);
    sem_destroy(&sems.s3);
    sem_destroy(&sems.s4);

    // Espere por TODAS as threads

    return 0;
}