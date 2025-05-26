// Leia o PDF ou MD antes de iniciar este exercício!

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    int ret;
    int wst;

    do {
        // Construa a chamada do executavel siscoin. Veja enunciado.

        char *prog = "./siscoin";
        char *args[] = {prog, argv[1], argv[2], argv[3], NULL};

        pid_t pid = fork();
        if (pid == 0) { // Processo filho
            execv(prog, args);
            perror("execv"); // Se execv falhar
            exit(EXIT_FAILURE);
        } else { // Processo pai
            waitpid(pid, &wst, 0); // Espera o filho terminar
            ret = WEXITSTATUS(wst); // Pega o status de saída do filho
        }

        // No pai, espere e trate o resultado

        // DICA: ao atribuir o return do siscoin, faça casting para (char)

    } while (ret > 0); // Repita enquanto houver falha. 
                       // Para se tem problema com o valor ou a qtde de parâmetros
                       // Esta validação está sendo feita em siscoin

    return 0;
}