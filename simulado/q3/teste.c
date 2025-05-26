#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // Para open() e flags
#include <unistd.h> // Para read(), write(), close()
#include <string.h> // Para strlen()
#include <errno.h>  // Para perror() e errno


void main(){
    int fd_escrita, fd_leitura;
    char *file_name = "pi.txt";
    char buffer_write[] = "3.14159265358979323846\n";
    char buffer_read[100];
    ssize_t bytes_written, bytes_read;

    // Abre o arquivo para escrita
    fd_escrita = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    printf("fd_escrita: %d\n", fd_escrita);

    // Escrever no arquivo

    bytes_written = write(fd_escrita, buffer_write, strlen(buffer_write));
    printf("bytes_written: %ld\n", bytes_written);

    // Fecha o arquivo de escrita
    close(fd_escrita);
    printf("Arquivo fechado.\n");

    // Abre o arquivo para leitura
    fd_leitura = open(file_name, O_RDONLY);
    printf("fd_leitura: %d\n", fd_leitura);

    // Lê do arquivo
    while((bytes_read = read(fd_leitura, buffer_read, sizeof(buffer_read)-1)) > 0){
        buffer_read[bytes_read] = '\0'; // Adiciona o terminador nulo
        printf("bytes_read: %ld\n", bytes_read);
        printf("Conteudo lido: %s", buffer_read);
    }

    if (bytes_read == -1) {
        perror("Erro ao ler o arquivo");
    }
    // Fecha o arquivo de leitura
    close(fd_leitura);
    printf("Arquivo fechado.\n");

    return;
}