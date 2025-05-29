// Leia o arquivo README.md ou README.html antes de iniciar este exercício!

// inclua libs aqui!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>     // Para strsignal e sprintf
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        
        printf("USAGE:\n./q2 <URL>\n");
        fflush(stdout);
        return 0;      
    }

    int curl_status_result; 
    int child_process_exit_status;
    int retries = 0;                    

    char *url = argv[1];
    char retries_str[12];

    do {
        
        sprintf(retries_str, "%d", retries);

        pid_t pid = fork();
        if (pid < 0) {
            perror("erro ao criar processo filho");
            return 1; 
        } else if (pid == 0) { 

            execl("./cURLInsper", "cURLInsper", url, retries_str, (char *)NULL);
            perror("erro ao executar cURLInsper");
            exit(127); 
        } else { 
            waitpid(pid, &child_process_exit_status, 0);

            // Inicializa com um valor que indica falha, para o loop continuar caso algo inesperado ocorra.
            curl_status_result = -100; // Um valor arbitrário de falha.

            if (WIFSIGNALED(child_process_exit_status)) {
                // (20% da nota)
                printf("CURLINSPER TERMINOU COM SINAL %s\n", strsignal(WTERMSIG(child_process_exit_status)));
                // curl_status_result já é um valor de falha, o loop continuará.
            } else if (WIFEXITED(child_process_exit_status)) {
                int exit_code = WEXITSTATUS(child_process_exit_status);

                if (exit_code > 0 && exit_code <= 127) { // Sucesso (custo > 0, geralmente <= 127 para exit codes)
                    printf("CURLINSPER TERMINOU NORMALMENTE!\n");
                    fflush(stdout); 
                    curl_status_result = exit_code;
                } else if (exit_code == 254) { // -2 (Falha de rede) -> WEXITSTATUS retorna 254
                    
                    printf("FALHA NO DOWNLOAD DA URL\n");
                    fflush(stdout); 
                    curl_status_result = -2;
                } else if (exit_code == 255) { // -1 (Argumentos incorretos para cURLInsper) -> WEXITSTATUS retorna 255
                    curl_status_result = -1;
                }
            }
            retries++; 
                       
        }
    } while (curl_status_result <= 0); 
    printf("DOWNLOAD COM SUCESSO, CUSTO TOTAL DE %d\n", retries * curl_status_result);
    fflush(stdout);

    return 0;
}