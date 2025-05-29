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
    int num_threads = args->n_threads;
    int qtd_nums = args->qtd_numeros;
    double *vetor = args->vetor;

    pthread_mutex_t *mutex = args->mutex;
    sem_t *sem = args->sem;

    int qtd_leitura = qtd_nums/num_threads;

    double soma_parcial = 0;
    int start = id*qtd_leitura;
    int end = (id+1)*qtd_leitura;

    int resto = qtd_nums%num_threads;

    if(resto > 0){
        if(id < resto){
            start += id;
            end += id+1;
        }
        else{
            start+= resto;
            end += resto;
        }
    }

    printf("id: %d, start: %d, end:%d\n", id, start, end);

    for(int i = start; i<end; i++){
        soma_parcial += vetor[i];        
    }

    pthread_mutex_lock(mutex);
    var_soma_global += soma_parcial;
    conta_threads++;
    pthread_mutex_unlock(mutex);

    if(conta_threads == num_threads){
       for (int i = 0; i < conta_threads; i++){
            sem_post(sem);
        }
    } else {
        sem_wait(sem);
    }

    if(id == 0){
        printf("media global: %lf\n", var_soma_global/qtd_nums);
    }

    return NULL;
}
