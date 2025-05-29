// Leia o arquivo README.md ou README.html antes de iniciar este exercício!

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>



typedef struct {
    pthread_mutex_t mutex;
    sem_t sem1;
    int qtd_threads;
    int num;
    // Preencha os campos da sua sua estrutura para passar como argumento para as threads
}t_arg;

void *sensor(void *arg) {
    t_arg *s = arg;

    // Aqui simulamos a coleta de dados - NÃO altere este print
    pthread_mutex_lock(&s->mutex);
    printf("Coleta de dados pelo sensor\n");
    s->num++;

    if(s->num == s->qtd_threads){
        for(int i=0; i<s->qtd_threads; i++){
            sem_post(&s->sem1);
        }
    }

    pthread_mutex_unlock(&s->mutex);

    // implemente aqui a sincronização das threads de forma que só
    // as threads so comecem a processas depois que todas concluiram a coleta
   

   sem_wait(&s->sem1);
    
    // Aqui simula o processamento envio - NÃO altere este print
    pthread_mutex_lock(&s->mutex);
    printf("Processa e envia dados\n");
    pthread_mutex_unlock(&s->mutex);
    return NULL;
}

int main(int argc, char *argv[])
{
  
    int qtd_threads = atoi(argv[1]);
    pthread_t *id_threads=malloc(qtd_threads*sizeof(pthread_t));

    t_arg *arg = malloc(sizeof(t_arg));
    arg->qtd_threads = qtd_threads;
    arg->num = 0;
    sem_init(&arg->sem1, 0, 0);
    pthread_mutex_init(&arg->mutex, NULL);
    
    // Declare e inicialize as estruturas para sincronizar as threads e 
    // em seguida crie as threads para executar a funcao sensor
    for(int i=0;i<qtd_threads;i++){
        pthread_create(&id_threads[i], NULL, sensor, arg);
    }


    // Espere por TODAS as threads terminarem
    for(int i=0;i<qtd_threads;i++){
        pthread_join(id_threads[i],NULL);
        sem_destroy(&arg->sem1);
        pthread_mutex_destroy(&arg->mutex);
        //free(&id_threads[i]);
    }

    free(arg);
    free(id_threads);
 
    printf("fim de programa principal.\n");
    return 0;
}