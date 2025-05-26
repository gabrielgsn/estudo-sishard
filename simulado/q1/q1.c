// Leia o PDF ou MD antes de iniciar este exercício!
// gcc q1.c -o q1 -pthread

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

// Faça os includes necessários aqui
// #include ...

// SÓ PODE USAR A ESTRUTURA ABAIXO, NAO PODE USAR VARIAVEIS GLOBAIS
struct semaforos {
    sem_t barrier;
    sem_t sem_t2, sem_t3, sem_t4;
    pthread_mutex_t mutex;
    // Declare aqui os campos da sua sua estrutura para passar como argumento para as threads
};

void *thread1(void *_arg) {
    struct semaforos *sems = (struct semaforos *)_arg;

    pthread_mutex_lock(&sems->mutex);
    pthread_mutex_unlock(&sems->mutex);

    printf("A\n");

    sem_post(&sems->sem_t2);
    sem_post(&sems->sem_t3);
    sem_post(&sems->sem_t4);

    sem_wait(&sems->barrier);
    sem_wait(&sems->barrier);
    sem_wait(&sems->barrier);

    pthread_mutex_lock(&sems->mutex);
    printf("E\n");
    pthread_mutex_unlock(&sems->mutex);

    return NULL;
}

void *thread2(void *_arg) {
    struct semaforos *sems = (struct semaforos *)_arg;

    sem_wait(&sems->sem_t2);
    pthread_mutex_lock(&sems->mutex);
    printf("B\n");
    pthread_mutex_unlock(&sems->mutex);
    sem_post(&sems->barrier);

    return NULL;
}

void *thread3(void *_arg) {
    struct semaforos *sems = (struct semaforos *)_arg;

    sem_wait(&sems->sem_t3);
    pthread_mutex_lock(&sems->mutex);
    printf("C\n");
    pthread_mutex_unlock(&sems->mutex);
    sem_post(&sems->barrier);

    return NULL;
}

void *thread4(void *_arg) {
    struct semaforos *sems = (struct semaforos *)_arg;
    sem_wait(&sems->sem_t4);
    pthread_mutex_lock(&sems->mutex);
    printf("D\n");
    pthread_mutex_unlock(&sems->mutex);
    sem_post(&sems->barrier);

    return NULL;
}

int main(int argc, char *argv[]) {

    // Crie TODAS as threads, voce deve utilizar semaforos para sincronizacao.
    pthread_t t1, t2, t3, t4;
    struct semaforos sems;
    sem_init(&sems.barrier, 0, 0);
    sem_init(&sems.sem_t2, 0, 0);
    sem_init(&sems.sem_t3, 0, 0);
    sem_init(&sems.sem_t4, 0, 0);

    // Inicialize os semaforos e mutexes
    pthread_mutex_init(&sems.mutex, NULL);
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
    sem_destroy(&sems.barrier);
    sem_destroy(&sems.sem_t2);
    sem_destroy(&sems.sem_t3);
    sem_destroy(&sems.sem_t4);
    pthread_mutex_destroy(&sems.mutex);
    
    
    return 0;
}
