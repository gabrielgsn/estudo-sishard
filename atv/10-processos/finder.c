#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NOT_FOUND 255

int main() {
    int x, n, k;
    
    // Le o elemento a ser buscado
    printf("Qual o elemento a ser buscado? ");
    if(scanf("%d", &x) != 1){
        fprintf(stderr, "Erro na leitura de x\n");
        exit(1);
    }
    
    // Le o tamanho do array
    printf("Qual o tamanho do array? ");
    if(scanf("%d", &n) != 1){
        fprintf(stderr, "Erro na leitura de n\n");
        exit(1);
    }
    
    int *array = malloc(n * sizeof(int));
    if(array == NULL){
        perror("Erro ao alocar memória");
        exit(1);
    }
    
    // Le os elementos do array
    printf("Digite os %d elementos do array:\n", n);
    for (int i = 0; i < n; i++){
        if(scanf("%d", &array[i]) != 1){
            fprintf(stderr, "Erro na leitura do elemento %d\n", i);
            free(array);
            exit(1);
        }
    }
    
    // Le a quantidade de processos
    printf("Qual a quantidade de processos? ");
    if(scanf("%d", &k) != 1){
        fprintf(stderr, "Erro na leitura de k\n");
        free(array);
        exit(1);
    }
    
    // Verifica se o array pode ser dividido igualmente entre os processos
    if(n % k != 0){
        fprintf(stderr, "Erro: n (tamanho do array) deve ser divisível por k (quantidade de processos).\n");
        free(array);
        exit(1);
    }
    
    int region_size = n / k;
    pid_t *child_pids = malloc(k * sizeof(pid_t));
    int *child_results = malloc(k * sizeof(int));
    if(child_pids == NULL || child_results == NULL){
        perror("Erro ao alocar memória para pids ou resultados");
        free(array);
        exit(1);
    }
    
    // Cria os processos filhos
    for (int i = 0; i < k; i++){
        pid_t pid = fork();
        if(pid < 0){
            perror("fork falhou");
            free(array);
            free(child_pids);
            free(child_results);
            exit(1);
        }
        else if(pid == 0){
            // Processo filho
            sleep(5);
            
            int start = i * region_size;
            int local_index = -1;
            for (int j = 0; j < region_size; j++){
                if(array[start + j] == x) {
                    local_index = j;  // indice local
                    break;
                }
            }
            
            // Libera os recursos antes de sair
            free(array);
            free(child_pids);
            free(child_results);
            
            // Caso não tenha encontrado
            if(local_index == -1)
                exit(NOT_FOUND);
            else
                exit(local_index);
        }
        else {
            // Processo pai
            child_pids[i] = pid;
            int start = i * region_size;
            int end = start + region_size;
            printf("Criou filho %d com PID=%d, r=[%d,%d)\n", i, pid, start, end);
        }
    }
    
    printf("Processo pai esperando os filhos finalizarem...\n");
    
    int final_process = -1;
    int final_index = -1;
    

    for (int i = 0; i < k; i++){
        int status;
        pid_t pid = wait(&status);
        
        int proc_index = -1;
        for (int j = 0; j < k; j++){
            if(child_pids[j] == pid){
                proc_index = j;
                break;
            }
        }
        
        int ret;
        if(WIFEXITED(status)){
            ret = WEXITSTATUS(status);
            if(ret == NOT_FOUND)
                ret = -1;
        } else {
            ret = -1;
        }
        child_results[proc_index] = ret == -1 ? -1 : ret+(region_size*proc_index);
        // printf("\n\n%d %d\n\n", i, region_size);
        printf("Processo %d com PID=%d finalizou retornando %d\n", proc_index, pid, ret);
    }
    
    // Seleciona o primeiro processo
    for (int i = 0; i < k; i++){
        if(child_results[i] != -1) {
            final_process = i;
            final_index = child_results[i];
            break;
        }
    }
    
    if(final_process != -1)
        printf("Elemento %d encontrado pelo processo %d no indice %d!\n", x, final_process, final_index);
    else
        printf("Elemento não encontrado!\n");
    
    free(array);
    free(child_pids);
    free(child_results);
    
    return 0;
}
