### `Bibliotecas`

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
```

### `Valgrind`

```c
valgrind --leak-check=yes ./executável
```

### `Threads (pthread.h)`

- Biblioteca para threads POSIX

```c
#include <pthread.h>
```

- Estrutura de Argumentos

```c
typedef struct {
    char *palavra;
    int res;

    // Outros campos conforme necessário (ex: semáforos)
} t_arg;
```

```c
pthread_t tid; // Variável para armazenar o ID da thread
t_arg args;    // Argumentos para a thread
```

- Criar thread

```c
pthread_create(&tid, NULL, nome_funcao_thread, (void *)&args);
```

- Esperar thread terminar

```c
pthread_join(tid, NULL);
```

### `Semáforos (semaphore.h)`

```c
#include <semaphore.h> 
````
```c
sem_t meu_semaforo;
````


- Inicializar semáforo
```c
sem_init(&meu_semaforo, 0, valor_inicial);

// pshared = 0: semáforo compartilhado entre threads do mesmo processo
// value: valor inicial do semáforo

````
```c
sem_wait(&meu_semaforo);
````
```c
sem_post(&meu_semaforo);
`````

```c
sem_destroy(&meu_semaforo);
````


```c
struct semaforos {
sem_t sem_t1_pode_iniciar;
sem_t sem_t2_pode_iniciar;
// ... outros semáforos
pthread_mutex_t mutex_exemplo; // Mutex também pode estar na struct
};

````
### `Mutex`

```c
pthread_mutex_t meu_mutex;
````

```c
pthread_mutex_init(&meu_mutex, NULL);
`````
```c
pthread_mutex_lock(&meu_mutex);

printf("Operação crítica...\n");

pthread_mutex_unlock(&meu_mutex);
`````
```c
pthread_mutex_destroy(&meu_mutex);
````

### `Processos`
```c
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
````
```c
pid_t pid_filho;
int status_filho;
````
```c
pid_filho = fork();
````
```c
if (pid_filho == 0) {
char \*args_para_exec[] = {"./outro_programa", "arg1", "arg2", NULL};
execv(args_para_exec[0], args_para_exec);

perror("Erro no execv"); // execv só retorna em caso de erro
exit(1); // Termina o filho em caso de erro no execv
} else if (pid_filho > 0) {
// Código do processo pai
// Esperar o filho terminar e obter status
wait(&status_filho);

    if (WIFEXITED(status_filho)) {
        int codigo_saida_filho = WEXITSTATUS(status_filho);
        printf("Filho terminou com status: %d\n", codigo_saida_filho);
    }

} else {
// Erro ao criar processo filho
perror("Erro no fork");
}
`````

```c
// Obter o ID do processo atual
pid_t meu_pid = getpid();
printf("Meu PID: %d\n", meu_pid); 
````

### `Sinais (signal.h)`
```c
#include <signal.h> // signal, SIGINT, etc.
#include <unistd.h> // pause (opcional, para esperar por um sinal)
````
```c
// Função handler para um sinal
void meu_handler_sigint(int signum) {
if (signum == SIGINT) {
printf("Recebido SIGINT (Ctrl+C)! Terminando...\n");
// Ações de limpeza, salvar dados, etc.
exit(0);
}
}

````
```c
int main() {
    ....
    /* Dentro da main, uma das primeiras coisas que fazemos é
    registrar nosso handler */
    struct sigaction s;
    s.sa_handler = sig_handler; // aqui vai a função a ser executada
    sigemptyset(&s.sa_mask);
    s.sa_flags = 0;

    sigaction(SIGINT, &s, NULL);
    ....
}
```

Exemplo Aula 17 questao sinais1.c:

```c
#include <stdio.h>
#include <unistd.h>
#include <signal.h> // Necessário para sigaction(), SIGTSTP, SIG_DFL, raise(), SIGCONT

// Fora da main, criamos a função que será nosso handler para SIGTSTP
void sigtstp_handler(int num) {
    // Verifica se o sinal recebido é SIGTSTP
    if (num == SIGTSTP) {
        // 1. Imprime a mensagem antes de parar
        printf("\nRecebi SIGTSTP (Ctrl+Z)! Vou parar agora...\n");
        fflush(stdout); // Garante que a mensagem seja impressa imediatamente

        // 2. Restaura o comportamento padrão do sinal SIGTSTP
        // Criamos uma nova estrutura sigaction para o comportamento padrão,
        // inicializando explicitamente seus campos.
        struct sigaction sa_default;
        sa_default.sa_handler = SIG_DFL;            // Define o handler como o padrão
        sigemptyset(&sa_default.sa_mask);           // Limpa a máscara (não bloqueia sinais adicionais)
        sa_default.sa_flags = 0;                    // Nenhuma flag especial para o comportamento padrão

        // Aplica a nova configuração (comportamento padrão) para SIGTSTP
        if (sigaction(SIGTSTP, &sa_default, NULL) == -1) {
            perror("Erro ao restaurar o handler padrão para SIGTSTP");
            // Em um programa real, você lidaria com este erro.
            // Para este exemplo, apenas imprimimos.
        }

        // 3. Reenvia o sinal SIGTSTP para o próprio processo.
        // Como o handler foi restaurado para o padrão (SIG_DFL), este raise()
        // fará com que o processo seja interrompido (parado) pelo shell.
        raise(SIGTSTP);
    }
}

// Nova função handler para o sinal SIGCONT
void sigcont_handler(int num) {
    // Verifica se o sinal recebido é SIGCONT
    if (num == SIGCONT) {
        printf("Continuando!\n");
        fflush(stdout); // Garante que a mensagem seja impressa imediatamente
        // Para SIGCONT, não precisamos restaurar o comportamento padrão ou reenviar o sinal,
        // pois seu objetivo já é continuar o processo.
    }
}

int main() {
    /* Dentro da main, uma das primeiras coisas que fazemos é
    registrar nossos handlers usando sigaction */

    // Configura e registra o handler para SIGTSTP
    struct sigaction sa_tstp;
    sa_tstp.sa_handler = sigtstp_handler;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = 0;
    if (sigaction(SIGTSTP, &sa_tstp, NULL) == -1) {
        perror("Erro ao definir o handler para SIGTSTP");
        return 1; // Indica que houve um erro na configuração
    }

    // Configura e registra o handler para SIGCONT
    struct sigaction sa_cont;
    sa_cont.sa_handler = sigcont_handler;
    sigemptyset(&sa_cont.sa_mask);
    sa_cont.sa_flags = 0;
    if (sigaction(SIGCONT, &sa_cont, NULL) == -1) {
        perror("Erro ao definir o handler para SIGCONT");
        return 1; // Indica que houve um erro na configuração
    }

    printf("Meu pid: %d\n", getpid());

    while(1) {
        printf("Rodando...\n");
        sleep(1);
    }
    return 0;
}
```


### `Entrada/Saída de Arquivos (fcntl.h, unistd.h)`
```c

#include <fcntl.h> // open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h> // read, write, close
`````
```c
int fd; // Descritor de arquivo
char buffer[256];
ssize_t bytes_lidos, bytes_escritos;
````

```c
// Abrir arquivo para leitura
fd = open("meu_arquivo.txt", O_RDONLY);
if (fd == -1) {
perror("Erro ao abrir arquivo para leitura");
}
````

```c
// Ler do arquivo
bytes_lidos = read(fd, buffer, sizeof(buffer) - 1);
if (bytes_lidos > 0) {
buffer[bytes_lidos] = '\0'; // Adicionar terminador nulo
printf("Lido: %s\n", buffer);
} else if (bytes_lidos == -1) {
perror("Erro ao ler arquivo");
}
close(fd); // Fechar após o uso
`````
```c
// Abrir/criar arquivo para escrita (sobrescreve se existir)
// 0644 são as permissões (rw-r--r--)
fd = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
if (fd == -1) {
perror("Erro ao abrir/criar arquivo para escrita");
}
`````
```c
// Escrever no arquivo
char \*mensagem = "Olá, mundo!\n";
bytes_escritos = write(fd, mensagem, strlen(mensagem));
if (bytes_escritos == -1) {
perror("Erro ao escrever no arquivo");
}
close(fd);
````


### `Alocação Dinâmica de Memória (stdlib.h)`

```c
#include <stdlib.h>
````
```c
// Alocar memória
char _str = (char _)malloc(100 \* sizeof(char)); // Aloca para 100 chars
if (str == NULL) {
perror("Erro no malloc");
exit(1);
}
`````
```c
// Realocar memória (aumentar ou diminuir)
char _str_realloc = (char _)realloc(str, 200 \* sizeof(char));
if (str_realloc == NULL) {
perror("Erro no realloc");
free(str); // Liberar o bloco original se realloc falhar
exit(1);
}
str = str_realloc; // Atualizar ponteiro
`````
```c
// Liberar memória alocada
free(str);
str = NULL; // Boa prática para evitar double free ou dangling 
```
### `Funções`

```c
// stdio.h
int ret_snprintf = snprintf(buffer, tam_buffer, "Valor: %d", valor_int); // Formatar string
// Cuidado com o tamanho do buffer para evitar overflow.
````
```c
// stdlib.h
int num = atoi("123"); // Converter string para inteiro
srand(time(NULL)); // Inicializar gerador de números aleatórios (q3.c, q4.c)
int r = rand(); // Gerar número aleatório (q4.c)
// double r_double = random() / (double)RAND_MAX; // Gerar double entre 0.0 e 1.0 (q3.c)
`````
```c
// unistd.h
sleep(segundos); // Suspender execução por 'segundos' (q3.c, q4.c)
`````
```c
// string.h
size_t len = strlen(const_char_ptr); // Obter tamanho da string
````
