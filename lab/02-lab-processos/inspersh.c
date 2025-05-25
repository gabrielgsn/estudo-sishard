/*
para compilar use:
gcc inspersh.c -Wall -Wno-unused-result -g -Og -o inspersh

para checar problemas de memoria:
valgrind --leak-check=yes ./inspersh
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

#define MAX_LINE 1024
#define MAX_TOKENS 8
#define MAX_HISTORY 100
#define MAX_ENTRIES 128

// ================== FASE 1 =====================

char initial_dir[MAX_LINE];

void print_prompt() {
    /* 
        funcao para exibir o prompt no formato (pid=<pid>)<path>$
        - <pid> eh o ID do processo atual
        - <path> eh o caminho completo do diretorio atual
    */

    char cwd[MAX_LINE];
    pid_t pid = getpid();
    getcwd(cwd, sizeof(cwd));

    printf("(pid=%d)%s$ ", pid, cwd);
    fflush(stdout);
}

int parse_input(char *line, char **tokens) {
    /*
        funcao para dividir a entrada em tokens
        - usa strtok para dividir a linha em tokens usando espacos e nova linha como delimitadores
        - armazena os tokens no array *tokens
        - retorna o numero de tokens encontrados
    */

    int token_count = 0;
    char *token = strtok(line, " \n");
    
    while (token != NULL && token_count < MAX_TOKENS) {
        tokens[token_count++] = token;
        token = strtok(NULL, " \n");
    }
    
    tokens[token_count] = NULL;
    return token_count;
}

void handle_cd(char **tokens, int token_count) {
    /*
        funcao para tratar o comando cd
        - altera o diretorio atual para o especificado no argumento
        - se nn tiver argumento, usa o diretorio inicial
    */

    char *dir = (token_count > 1) ? tokens[1] : initial_dir;

    int status = chdir(dir);

    if (status != 0){
        printf("diretorio %s nao encontrado \n", dir);
    }
}

void handle_ls() {
    /*
        funcao para tratar o comando ls
        - lista os arquivos e diretorios no diretorio atual
        - exibe o tipo (d ou a), tamanho (em bytes ou '-') e nome
    */

    DIR *dir = opendir(".");
    
    struct dirent *entry;
    struct stat file_stat;
    
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue; // pra nn pegar arquivos hidden
        }
        
        stat(entry->d_name, &file_stat);
        
        char type = S_ISDIR(file_stat.st_mode) ? 'd' : 'a';
        char *size = S_ISDIR(file_stat.st_mode) ? "-" : "";
        if (S_ISREG(file_stat.st_mode)) {
            char size_buf[32];
            snprintf(size_buf, sizeof(size_buf), "%lld", (long long)file_stat.st_size);
            size = size_buf;
        }
        
        printf("%c - %s - %s\n", type, size, entry->d_name);
    }
    closedir(dir);
}

// ================== FASE 2 =====================

char *current_cmd = NULL;
pid_t child_pid = -1;

void handle_sigint(int sig) {
    /*
        funcao para tratar o sinal SIGINT (ctrl+c)
        - se tiver processo filho, envia SIGINT pra ele
        - senao ignora
    */

    if (child_pid > 0) {
        kill(child_pid, SIGINT);
        printf("\n(pid=%d):comando externo [%s] finalizado com ctrl+c.\n", child_pid, current_cmd);
        fflush(stdout);
    }
}

void handle_external(char **tokens) {
    /*
        funcao para tratar comandos externos
        - cria um processo filho com fork
        - executa o comando com execvp
        - aguarda o filho terminar e printa se deu certo
    */

    child_pid = fork();

    if (child_pid == 0) {
        // filho
        execvp(tokens[0], tokens);
    } else {
        // pai
        current_cmd = tokens[0];
        int status;
        waitpid(child_pid, &status, 0);
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            printf("(pid=%d):comando externo [%s] executado com sucesso.\n", child_pid, tokens[0]);
        }
        
        child_pid = -1;
        current_cmd = NULL;
    }
}

// ================== FASE 3 =====================

int history_fd;

void save_to_history(char *line) {
    /*
        funcao pra salvar o comando no arquivo historico.txt
        - escreve a linha inteira e uma nova linha
    */

    write(history_fd, line, strlen(line));
    write(history_fd, "\n", 1);
}

void execute_history(char **tokens, char *original_line) {
    /*
        funcao pra tratar comandos de historico (!, !#, !<prefixo>)
        - le o historico e busca o comando correspondente
        - reexecuta o comando encontrado
    */

    char history[MAX_HISTORY][MAX_LINE];
    int history_count = 0;
    
    lseek(history_fd, 0, SEEK_SET);
    
    char buffer[MAX_LINE];
    int bytes_read = 0;
    int posicao = 0;
    
    while ((bytes_read = read(history_fd, &buffer[posicao], 1)) > 0) {
        if (buffer[posicao] == '\n') {
            buffer[posicao] = '\0';
            strcpy(history[history_count], buffer);
            history_count++;
            posicao = 0;
        } else {
            posicao++;
        }
    }
    
    // se nn tiver historico, printa erro
    if (history_count == 0) {
        printf("(pid=%d):comando [%s] nao encontrado no historico.\n", getpid(), original_line);
        return;
    }
    
    char *cmd_to_do = NULL;
    
    if (strcmp(tokens[0], "!") == 0) {
        // !
        for (int i = 0; i < history_count; i++) {
            printf("%d\t%s\n", i, history[i]);
        }
        return;
    } else if (tokens[0][1] >= '0' && tokens[0][1] <= '9') {
        // !#
        int num = atoi(&tokens[0][1]); // str para int
        if (num >= 0 && num < history_count) {
            cmd_to_do = history[num];
            printf("%s\n", cmd_to_do);
        } else {
            printf("indice invalido\n");
            return;
        }
    } else {
        // !<prefixo>
        char prefix[MAX_LINE];
        strcpy(prefix, &tokens[0][1]);
        for (int i = history_count - 1; i >= 0; i--) {
            if (strncmp(history[i], prefix, strlen(prefix)) == 0) {
                cmd_to_do = history[i];
                printf("%s\n", cmd_to_do);
                break;
            }
        }
        if (cmd_to_do == NULL) {
            printf("comando nao encontrado\n");
            return;
        }
    }
    
    // executa o comando encontrado
    char cmd_copy[MAX_LINE];
    strcpy(cmd_copy, cmd_to_do);
    char *new_tokens[MAX_TOKENS + 1];
    int token_count = parse_input(cmd_copy, new_tokens);
    
    if (strcmp(new_tokens[0], "cd") == 0) {
        handle_cd(new_tokens, token_count);
    } else if (strcmp(new_tokens[0], "ls") == 0) {
        handle_ls();
    } else {
        handle_external(new_tokens);
    }

    // salva a reexecucao no historico
    save_to_history(cmd_to_do);
}

// ================== FASE 4 =====================

    // mt dificil luba :(

// ================== MAIN =====================

int main(int argc, char *argv[]) {
    char line[MAX_LINE];
    char *tokens[MAX_TOKENS + 1];
    
    signal(SIGINT, handle_sigint);
    
    getcwd(initial_dir, sizeof(initial_dir));
    
    history_fd = open(argv[1], O_CREAT | O_RDWR | O_APPEND, 0644);
    
    while (1) {
        print_prompt();
        
        ssize_t bytes_read = read(STDIN_FILENO, line, MAX_LINE - 1);
        if (bytes_read <= 0) {
            break;
        }

        line[bytes_read] = '\0';
        char *newline = strchr(line, '\n');
        if (newline) {
            *newline = '\0';
        }

        char line_copy[MAX_LINE];
        strcpy(line_copy, line);

        int token_count = parse_input(line, tokens);
        if (token_count == 0) {
            continue;
        }
        
        if (tokens[0][0] != '!' && strcmp(tokens[0], "exit") != 0) {
            save_to_history(line_copy);
        }
        
        if (tokens[0][0] == '!') {
            execute_history(tokens, line);
        } else if (strcmp(tokens[0], "exit") == 0) {
            break;
        } else if (strcmp(tokens[0], "cd") == 0) {
            handle_cd(tokens, token_count);
        } else if (strcmp(tokens[0], "ls") == 0) {
            handle_ls();
        } else {
            handle_external(tokens);
        }
    }
    
    close(history_fd);
    return 0;
}