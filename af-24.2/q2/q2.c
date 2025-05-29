// Leia o arquivo README.md ou README.html antes de iniciar este exercício!

// inclua libs aqui!
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

    int ret;
    int status;
    int times_run = 0;

    do {
        // Construa a chamada do executavel siscoin. Veja enunciado.

        char *prog = "./criptInsper";
        char n[100];
        sprintf(n, "%d", times_run);

        pid_t pid = fork();
        if (pid == 0) { // Processo filho
            execl(prog, "criptInsper", argv[1], n, (char *)NULL);
            perror("execv"); // Se execv falhar
            exit(EXIT_FAILURE);
        } else { // Processo pai
            waitpid(pid, &status, 0); // Espera o filho terminar
            if(WIFSIGNALED(status)){
                printf("CRIPTINSPER TERMINOU COM SINAL %s\n", strsignal(WTERMSIG(status)));
                fflush(stdout);
            }
            else{
                ret = WEXITSTATUS(status); // Pega o status de saída do filho

            }
        }
        times_run++;


    } while (status != 0); // Repita enquanto houver falha. 
                       // Para se tem problema com o valor ou a qtde de parâmetros
                       // Esta validação está sendo feita em siscoin

    printf("CRIPTINSPER TERMINOU NORMALMENTE!\n");
    printf("TIVEMOS UM GASTO TOTAL DE %lf\n", atof(argv[2])*times_run);


    return 0;
}