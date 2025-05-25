// gcc finderThread.c -o finderThread -pthread

// implemente aqui sua solução!

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


int* vetor_principal_global;
int valor_buscado_global;
int tamanho_vetor_global;
int num_total_threads_global;

void* thread_busca(void* argumento_thread) {
    long thread_id = (long)argumento_thread;

    sleep(5);

    int tamanho_regiao_por_thread = tamanho_vetor_global / num_total_threads_global;
    int idx_inicial_busca = thread_id * tamanho_regiao_por_thread;
    int idx_final_exc_busca = idx_inicial_busca + tamanho_regiao_por_thread;

    for (int i = idx_inicial_busca; i < idx_final_exc_busca; i++) {
        if (vetor_principal_global[i] == valor_buscado_global) {
            pthread_exit((void*)(long)i);
        }
    }

    pthread_exit((void*)-1L); // nao achou
}

int main() {
    int valor_a_buscar_local;
    int tamanho_vetor_local;
    int* vetor_A_local;
    int k_threads_local;

    printf("elemento a ser buscado? \n");
    if (scanf("%d", &valor_a_buscar_local) != 1) {
        fprintf(stderr, "erro elemento a ser buscado \n");
        return 1;
    }

    printf("tamanho do array? \n");
    if (scanf("%d", &tamanho_vetor_local) != 1 || tamanho_vetor_local <= 0) {
        fprintf(stderr, "erro tamanho do array \n");
        return 1;
    }

    vetor_A_local = (int*)malloc(tamanho_vetor_local * sizeof(int));
    if (vetor_A_local == NULL) {
        fprintf(stderr, "erro ao alocar memoria vetor \n");
        return 1;
    }

    printf("%d elementos do array? \n", tamanho_vetor_local);
    for (int i = 0; i < tamanho_vetor_local; i++) {
        if (scanf("%d", &vetor_A_local[i]) != 1) {
            fprintf(stderr, "erro elemento %d do array \n", i);
            free(vetor_A_local);
            return 1;
        }
    }

    printf("quantidade de threads? \n");
    if (scanf("%d", &k_threads_local) != 1 || k_threads_local <= 0) {
        fprintf(stderr, "erro quantidade de threads \n");
        free(vetor_A_local);
        return 1;
    }

    if (tamanho_vetor_local % k_threads_local != 0) {
        fprintf(stderr, "erro - tamanho do array (%d) nao eh divisivel pelo # de threads (%d) \n", tamanho_vetor_local, k_threads_local);
        free(vetor_A_local);
        return 1;
    }

    vetor_principal_global = vetor_A_local;
    valor_buscado_global = valor_a_buscar_local;
    tamanho_vetor_global = tamanho_vetor_local;
    num_total_threads_global = k_threads_local;

    int tamanho_regiao_calculado = tamanho_vetor_global / num_total_threads_global;

    pthread_t* ids_threads = (pthread_t*)malloc(num_total_threads_global * sizeof(pthread_t));
    if (ids_threads == NULL) {
        fprintf(stderr, "erro alocacao memoria para os id das threads \n");
        free(vetor_A_local);
        return 1;
    }

    for (long i = 0; i < num_total_threads_global; i++) {
        int inicio_regiao_thread_atual = i * tamanho_regiao_calculado;
        int fim_regiao_thread_atual = inicio_regiao_thread_atual + tamanho_regiao_calculado;

        printf("criou thread %ld, r=[%d,%d] \n",
               i,
               inicio_regiao_thread_atual,
               fim_regiao_thread_atual);

        if (pthread_create(&ids_threads[i], NULL, thread_busca, (void*)i) != 0) {
            perror("erro ao criar thread");
            free(vetor_A_local);
            free(ids_threads);
            return 1;
        }
    }

    printf("main() -> esperando %d as threads finalizarem \n", num_total_threads_global);

    long indice_encontrado_geral = -1;
    int id_thread_que_encontrou = -1;

    for (int i = 0; i < num_total_threads_global; i++) {
        void* ponteiro_resultado_thread;
        pthread_join(ids_threads[i], &ponteiro_resultado_thread);

        long resultado_indice_thread_atual = (long)ponteiro_resultado_thread;

        printf("thread %d finalizou retornando %ld \n", i, resultado_indice_thread_atual);

        if (resultado_indice_thread_atual != -1) {
            if (indice_encontrado_geral == -1 || resultado_indice_thread_atual < indice_encontrado_geral) {
                indice_encontrado_geral = resultado_indice_thread_atual;
                id_thread_que_encontrou = i;
            }
        }
    }

    if (indice_encontrado_geral != -1) {
        printf("elemento %d encontrado pela thread %d no indice %ld \n",
               valor_buscado_global, id_thread_que_encontrou, indice_encontrado_geral);
    } else {
        printf("elemento nao encontrado \n");
    }

    free(vetor_A_local);
    vetor_principal_global = NULL;
    free(ids_threads);

    return 0;
}