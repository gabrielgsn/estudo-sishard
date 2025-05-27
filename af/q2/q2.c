// Leia o arquivo README.md ou README.html antes de iniciar este exercício!

// inclua libs aqui!
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("USAGE:\n./q4 <URL>\n");
        return 0;
    }

    // SUA RESPOSTA AQUI
    int response;
    int status;
    int retries = 0;

    char *url = argv[1];

    do {
        pid_t pid = fork();
        if (pid < 0) {
            perror("erro ao criar processo filho");
            return -1;
        } else if (pid == 0) {
            execl("./cURLInsper", "./cURLInsper", url, retries, NULL);
            perror("erro ao executar cURLInsper"); // erro ao executar cURLInsper: Bad address (não faço a menor ideia por quê isso está acontecendo)
            return -1;
        } else {
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                response = (char)WEXITSTATUS(status);
            }

            if (response == -2) {
                printf("FALHA NO DOWNLOAD DA URL\n");
            } else if (response > 0) {
                printf("CURLINSPER TERMINOU NORMALMENTE!\n");
            }
        }
        retries++;
    } while (response <= 0);

    printf("DOWNLOAD COM SUCESSO, CUSTO TOTAL DE %d\n", retries * response);
    return 0;
}