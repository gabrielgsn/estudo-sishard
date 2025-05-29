// Leia o arquivo README.md ou README.html antes de iniciar este exercício!
// para compilar use:
// gcc q1.c -o q1 -pthread

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

typedef sem_t semt;
struct semaforos
{
    // Declare aqui os campos da sua sua estrutura para passar como argumento para as threads
    semt semaphore1;
    semt semaphore2;
    semt semaphore3;
    semt semaphore4;
    pthread_mutex_t mutex;
};

void *thread1(void *_arg){
    struct semaforos *sems = (struct semaforos *)_arg;
    printf("A\n");
    sem_post(&sems->semaphore1);
    sem_post(&sems->semaphore1);
    sem_post(&sems->semaphore1);

    
    sem_wait(&sems->semaphore2);
    sem_wait(&sems->semaphore3);
    sem_wait(&sems->semaphore4);
    printf("F\n");

    return NULL;
}

void *thread2(void *_arg){
    struct semaforos *sems = (struct semaforos *)_arg;
    sem_wait(&sems->semaphore1);
    sem_wait(&sems->semaphore3);
    printf("C\n");
    sem_post(&sems->semaphore2);
    sem_post(&sems->semaphore2);

    return NULL;
}

void *thread3(void *_arg){
    struct semaforos *sems = (struct semaforos *)_arg;
    printf("B\n");
    sem_post(&sems->semaphore3);
    sem_post(&sems->semaphore3);

    sem_wait(&sems->semaphore1);
    printf("D\n");

    sem_post(&sems->semaphore3);
    sem_post(&sems->semaphore3);

    return NULL;
}

void *thread4(void *_arg)
{
    struct semaforos *sems = (struct semaforos *)_arg;
    sem_wait(&sems->semaphore3);
    sem_wait(&sems->semaphore1);
    printf("E\n");
    sem_post(&sems->semaphore4);
   

    sem_wait(&sems->semaphore3);
    sem_wait(&sems->semaphore2);
    printf("G\n");

    return NULL;
}

int main(int argc, char *argv[])
{

    // Crie TODAS as threads. Voce deve utilizar semaforos para sincronizacao.
    pthread_t t1, t2, t3, t4;
    struct semaforos sems;
    sem_init(&sems.semaphore1, 0, 0);
    sem_init(&sems.semaphore2, 0, 0);
    sem_init(&sems.semaphore3, 0, 0);
    sem_init(&sems.semaphore4, 0, 0);

    pthread_create(&t1, NULL, thread1, &sems);
    pthread_create(&t2, NULL, thread2, &sems);
    pthread_create(&t3, NULL, thread3, &sems);
    pthread_create(&t4, NULL, thread4, &sems);
    // Espere por TODAS as threads
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);


    sem_destroy(&sems.semaphore1);
    sem_destroy(&sems.semaphore2);
    sem_destroy(&sems.semaphore3);
    sem_destroy(&sems.semaphore4);

    return 0;
}