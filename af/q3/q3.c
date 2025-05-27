// Leia o arquivo README.md ou README.html antes de iniciar este exercício!

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *le_palavra_arquivo(int fd) {
    char *word = malloc(22 * sizeof(char));
    int i = 0, n;
    char c;

    while (1) {
        n = read(fd, &c, 1);
        if (c != ' ') {
            word[i] = c;
            i++;
        } else if (c == '\0') {
            word[i] = c;
            return word;
        }
    }
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
    int output = open("q3_saida.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);

    if (output < 0) {
        perror("erro ao abrir arquivo");
        exit(1);
    }

    int espaços_extras;

    for (int i = 0; i < qtd_linhas; i++) {
        int tamanho_palavra = strlen(palavras[i]);
        if (tamanho_palavra < tamanho_max) {
            espaços_extras = tamanho_max - tamanho_palavra;

            char palavra_nova[50];
            int idx = 0;
            for (int j = 0; j < espaços_extras; j++) {
                palavra_nova[i] = ' ';
                idx = j;
            }
            for (int l = 0; l < tamanho_palavra; l++) {
                char letra = palavras[i][l];
                palavra_nova[idx++] = letra;
            }
            if (write(output, palavra_nova, strlen(palavra_nova)) < 0) {
                perror("erro ao escrever no arquivo");
                close(output);
                exit(1);
            }
        } else {
            if (write(output, palavras[i], tamanho_palavra) < 0) {
                perror("erro ao escrever no arquivo");
                close(output);
                exit(1);
            }
        }
    }

    close(fdIn);
    close(output);
    free(palavras);
    return 0;
}
