// Leia o arquivo README.md ou README.html antes de iniciar este exercício!

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

struct semaforos {
    // Preencha os campos da sua sua estrutura para passar como argumento para as threads
};
void *sensor(void *arg) {
    struct semaforos *sem_barreira = arg;

    // Aqui simulamos a coleta de dados - NÃO altere este print
    printf("Coleta de dados pelo sensor\n");

    // implemente aqui a sincronização das threads de forma que só
    // as threads so comecem a processas depois que todas concluiram a coleta
    

    // Aqui simula o processamento envio - NÃO altere este print
    printf("Processa e envia dados\n");
    return NULL;
}

int main(int argc, char *argv[])
{
  
    if (argc < 2)
    {
        printf("Use:\n./q4 qtd_sensores\n");
        return EXIT_FAILURE;
    }

    int qtd_threads = atoi(argv[1]);
    pthread_t *id_threads=malloc(qtd_threads*sizeof(pthread_t));

    // Declare e inicialize as estruturas para sincronizar as threads e 
    // em seguida crie as threads para executar a funcao sensor

    // Espere por TODAS as threads terminarem
 
    printf("fim de programa principal.\n");
    return 0;
}
