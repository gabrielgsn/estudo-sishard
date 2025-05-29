// Leia o enunciado antes de iniciar este exercício!

// inclua libs aqui!
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h> // Para open() e flags
#include <string.h> // Para strlen()
#include <pthread.h> // Para pthreads
#include <semaphore.h> // Para semáforos

sem_t sem;

typedef struct
{
    char *palavra; // palavra a ser classificada
    int res;       // retorno da thread virá aqui!
} t_arg;

#define MAX_LINE_LENGTH 256

/*
 * Função que simula a classificação de uma palavra
 */
int classifier(char *palavra)
{
    int sleep_time = rand() % 1 + 4;

    // Espera por uma qtde aleatória de segundos
    sleep(sleep_time);

    int soma = 157;

    for (int i = 0; palavra[i] != '\0'; i++)
    {
        soma += palavra[i];
    }

    int res = (soma % 3);

    return res;
}

/*
 * Classificação em Threads!
 */
void *classifier_thread(void *_arg)
{
    t_arg *arg = _arg;

    sem_wait(&sem); // Aguarda a liberação do semáforo
    arg->res = classifier(arg->palavra);
    sem_post(&sem); // Libera o semáforo

    pthread_exit(NULL);

}

/* Função que lê uma palavra de um arquivo.
 * Considere que cada linha do arquivo contem apenas uma palavra.
 *
 * Entradas:
 *   int fd: descritor do arquivo
 *
 * Saída:
 *   Ponteiro (char *) para uma string com a palavra lida (sem \n). Caso o arquivo termine, retorne NULL.
 */
char *read_line(int fd)
{
    char *buffer = malloc(MAX_LINE_LENGTH+1);
    int i = 0;
    char c;
    ssize_t bytes_read;
    while (i<MAX_LINE_LENGTH)
    {
        bytes_read = read(fd, &c, 1);

        if (bytes_read == 0)
        {
            if(i==0){
                free(buffer);
                return NULL;
            }
            else{
                break;
            }
        }

        if (c == '\n')
        {
            break;
        }

        buffer[i++] = c;
    }

    buffer[i] = '\0';

    if (buffer)
    
    return buffer;  
}

int main(int argc, char *argv[]){
    
    if (argc != 3)
    {
        printf("USAGE:\n./q4 <n_threads> <file_name>\n");
        return 0;
    }

    int n_threads = atoi(argv[1]); // Info da linha de comando

    sem_init(&sem, 0, n_threads); // Inicializa o semáforo com o número de threads


    char *file_name = argv[2];     // Info da linha de comando

    int fd1 = open(file_name, O_RDONLY);

    if (fd1 == -1)
    {
        fprintf(stderr, "Falha ao abrir o arquivo!\n");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    int threads_limit = 2; // Vamos começar alocando espaço para 02 threads
    pthread_t *tids = malloc(sizeof(pthread_t) * threads_limit);
    t_arg **args = malloc(sizeof(t_arg *) * threads_limit);

    int threads_created = 0; // Quantas threads criei de fato

    for (int i = 0;; i++)
    {
        // Tenta ler uma palavra
        char *palavra = read_line(fd1);

        // Para quando não conseguir mais ler
        if (palavra == NULL)
        {
            break;
        }

        t_arg *arg = malloc(sizeof(t_arg));
        // Passa a palavra para a thread
        arg->palavra = palavra;

        // Realoca se necessário
        if (threads_limit == threads_created)
        {
            threads_limit *= 2;
            tids = realloc(tids, sizeof(pthread_t) * threads_limit);
            args = realloc(args, sizeof(t_arg *) * threads_limit);
        }

        args[i] = arg;

        pthread_create(&tids[i], NULL, classifier_thread, arg);
        printf("CRIOU THREAD %02d PARA PROCESSAR A PALAVRA %s\n", i, arg->palavra);

        threads_created++;
    }
    // Complete a main!

    int classes[3] = {0, 0, 0};

    for(int i=0; i<threads_created; i++){
        pthread_join(tids[i], NULL);
        printf("THREAD %02d TERMINOU COM O RESULTADO %d\n", i, args[i]->res);
        classes[args[i]->res]++;

        free(args[i]->palavra);
        free(args[i]);
    }

    printf("RESULTADO:\n");
    printf("TOTAL CLASSE 0: %d\n", classes[0]);
    printf("TOTAL CLASSE 1: %d\n", classes[1]);
    printf("TOTAL CLASSE 2: %d\n", classes[2]);

    free(tids);
    free(args);

    sem_destroy(&sem); // Destroi o semáforo
    close(fd1); // Fecha o arquivo

    return 0;
}
