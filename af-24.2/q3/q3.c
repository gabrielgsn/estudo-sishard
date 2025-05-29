// Leia o arquivo README.md ou README.html antes de iniciar este exercício!
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_LINHA 100

int my_strlen(const char *s) {
    int len = 0;
    if (s == NULL) return 0; // Proteção contra ponteiro nulo
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

// Implemente aqui sua funcao que le uma linha do arquivo
char *le_linha_arquivo(int fd) {
    char *buffer = malloc(MAX_LINHA+1);
    int i = 0;
    char c;
    ssize_t bytes_read;
    while (i<MAX_LINHA)
    {
        bytes_read = read(fd, &c, 1);

        if (bytes_read == 0)
        {
            if(i==0){
                free(buffer);
                return NULL;
            }
            else{
                break;
            }
        }

        if (c == '\n')
        {
            break;
        }

        buffer[i++] = c;
    }

    buffer[i] = '\0';

    if (buffer)

    printf("BUFFER: %s\n", buffer);
    
    return buffer;

}
int main(int argc, char *argv[])
{
  
    if (argc < 2)
    {
        printf("Use:\n./q3 arquivo_entrada\n");
        return EXIT_FAILURE;
    }

    int fdIn = open(argv[1], O_RDONLY);
    int fdOut= open("q3_saida.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);      
    
    // Escreva a partir daqui o trecho que elimina a primeira e a ultima palavra
    // do arquivo informado por linha de comando
    // Considere que sempre teremos pelo menos duas palavras no arquivo


    for(int i=0;; i++){
        char *palavra = le_linha_arquivo(fdIn);
        char *new_word = malloc(MAX_LINHA);
        int idx = 0;

        if(palavra == NULL){
            free(palavra);
            free(new_word);
            break;
        }


        int first = -1;
        int last = 0;
        int tamanho = 0;

        for(int j=0;; j++){
            if(palavra[j] == ' ' && first == -1){
                first = j;
                printf("Posicao First: %d\n", first);
            }
            if(palavra[j] == ' '){
                last = j;
                printf("Posicao Last: %d\n", last);
            }
            if(palavra[j] == '\0'){
                break;
            }
            tamanho++;
        }

        int diff = last-first;
        printf("diff: %d\n", diff);

        if(diff == 0){
            //
        }
        else{
            for(int l = first+1; l<last; l++){
                new_word[idx] = palavra[l];
                idx++;
            }

        }

        new_word[idx++] = '\n';
        new_word[idx] = '\0';


        printf("PALAVRA: %s\n", new_word);

        ssize_t bytes_written = write(fdOut, new_word, my_strlen(new_word));
        printf("Escreveu %ld bytes\n\n", bytes_written);

        free(palavra);
        free(new_word);
    
    }

    close(fdOut);
    close(fdIn);

    return 0;
}

