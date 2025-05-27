// para compilar use
// gcc -g -Og -Wall q4.o solucao.c -o q4 -pthread
// Leia com atenção README.md ou README.html antes de iniciar este exercício!
#include "q4.h"

// variaveis globais
extern double var_soma_global;
extern int conta_threads;

void *soma_parcial(void *_arg) {
    struct soma_parcial_args *args = _arg;
    int id = args->id_thread;
    int n_threads = args->n_threads;
    int qtd_numeros = args->qtd_numeros;
    double *vetor = args->vetor;
    pthread_mutex_t *mutex = args->mutex;
    sem_t *sem = args->sem;

    double soma_parcial = 0.0;
    int start = (qtd_numeros / n_threads) * id;
    int end = (id == n_threads - 1) ? qtd_numeros : start + (qtd_numeros / n_threads);

    for (int i = start; i < end; i++) {
        soma_parcial += vetor[i];
    }

    pthread_mutex_lock(mutex);
    var_soma_global += soma_parcial;
    conta_threads++;
    pthread_mutex_unlock(mutex);

    // libera semáforo após todas as threads terminarem as somas
    if (conta_threads == n_threads) {
        sem_post(sem);
    }

    // esperar semáforo para imprimir a média
    sem_wait(sem);
    sem_post(sem);

    if (id == 0) { // apenas uma thread imprime a média
        printf("MEDIA: %.3lf\n", var_soma_global / qtd_numeros);
    }

    return NULL;
}
