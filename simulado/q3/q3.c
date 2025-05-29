// Leia o PDF ou MD antes de iniciar este exercício!

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h> // Para open() e flags
#include <string.h> // Para strlen()
#include <signal.h> // Para sinal

// Variável GLOBAL para armazenar o valor de PI aproximado
double pi = 0.0;

// Função que gera um numero aleatorio uniformemente entre 0.0 e 1.0
// Você não deve alterar esta função
double random_zero_one() {
    return (double)random()/(double)RAND_MAX;
}

// Função que calcula o valor de pi por simulação (Monte Carlo)
// Você não deve alterar esta função
double aproxima_pi() {
    long dentro = 0;
    long total_pontos = 0;

    double x,y,d;
    while (1) {
        x = random_zero_one();
        y = random_zero_one();
        d = x*x + y*y;
        if (d<=1) {
            dentro++;
        }
        total_pontos++;
        pi = 4.0 * ((double)dentro/(double)total_pontos);
        if (random_zero_one() < 0.008) {
            sleep(1);
        }
    }
}

// Crie AQUI a função que exporta o valor de pi para um arquivo chamado pi.txt

void export_pi(char file_name[], char buffer_write[]) {
    // Abre o arquivo para escrita
    int fd_escrita = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    printf("fd_escrita: %d\n", fd_escrita);

    // Escrever no arquivo
    ssize_t bytes_written = write(fd_escrita, buffer_write, strlen(buffer_write));
    printf("bytes_written: %ld\n", bytes_written);

    // Fecha o arquivo de escrita
    close(fd_escrita);
    printf("Arquivo fechado.\n");
    
}

// Esta função deve ser chamada pelo handler quando este for acionado

pid_t pid; // Variável global para armazenar o PID do processo filho

// Crie AQUI a função que será o handler do sinal
void handler(int sig) {
    printf("Ctrl-C recebido, exportando pi = %f\n", pi);
    char file_name[] = "pi.txt";
    char buffer_write[1000];
    snprintf(buffer_write, sizeof(buffer_write), "%f\n", pi);
    export_pi(file_name, buffer_write);
    kill(pid, SIGKILL);
}


int main() {
    pid = getpid(); // Armazena o PID do processo atual
    // Exiba o PID deste processo
    printf("PID do processo: %d\n", getpid());

    // Registre AQUI seu handler para o sinal SIGINT!
    signal(SIGINT, handler);

    
    srand(time(NULL));
    aproxima_pi();

    return 0;
}