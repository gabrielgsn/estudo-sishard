#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

struct soma_parcial_args {
    int id_thread; // indice da thread
    int n_threads; // numero de threads
    int qtd_numeros; // qtd numeros do arquivo de entrada
    double *vetor; // vetor de numeros
    // variaeis de sincronizacao das threads
    pthread_mutex_t *mutex; 
    sem_t  *sem;
};

void *soma_parcial(void *_arg);