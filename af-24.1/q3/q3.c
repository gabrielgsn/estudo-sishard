// Leia o arquivo README.md ou README.html antes de iniciar este exercício!

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char *le_palavra_arquivo(int fd) {
    char *buffer = malloc(1024);
    if (!buffer) {
        perror("malloc failed in read_line");
        return NULL;
    }

    int i = 0;
    char c;
    ssize_t bytes_read;

    while (i < 1024)
    {
        bytes_read = read(fd, &c, 1);

        if (bytes_read == -1) {
            perror("read failed in read_line");
            free(buffer);
            return NULL;
        }

        if (bytes_read == 0)
        {
            if (i == 0) {
                free(buffer);
                return NULL;
            }
            else {
                break;
            }
        }

        if (c == '\n')
        {
            break;
        }
        if(c != ' '){
            buffer[i++] = c;

        }
        // buffer[i++] = c;
    }

    buffer[i] = '\0';
    return buffer;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("USAGE:\n./q4 num_linhas arquivo_entrada\n");
        return EXIT_FAILURE;
    }
    int fdIn = open(argv[2], O_RDONLY);
    int qtd_linhas = atoi(argv[1]);

    int tamanho_max = 0;

    char **palavras = malloc(qtd_linhas * sizeof(char *));
    for (int i = 0; i < qtd_linhas; i++) {
        palavras[i] = le_palavra_arquivo(fdIn);
        printf("LIDA A PALAVRA [%s]\n", palavras[i]);
        if (strlen(palavras[i]) > tamanho_max) {
            tamanho_max = strlen(palavras[i]);
        }
    }
    // ESCREVA AQUI A PARTE QUE JUSTIFICA AS PALAVRAS
    int fdOut = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

    for(int i=0; i<qtd_linhas; i++){
        int num_espacos = tamanho_max - strlen(palavras[i]);
        for(int j = 0; j<num_espacos; j++){
            write(fdOut, " ", sizeof(char));
        }
        write(fdOut, palavras[i], strlen(palavras[i]));
        write(fdOut, "\n", sizeof(char));
    }
    
    close(fdIn);
    for(int i=0; i<qtd_linhas; i++){
        free(palavras[i]);
    }
    free(palavras);
    
    return 0;
}
