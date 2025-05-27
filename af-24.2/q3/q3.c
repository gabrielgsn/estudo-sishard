// Leia o arquivo README.md ou README.html antes de iniciar este exercício!
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_LINHA 100

// Implemente aqui sua funcao que le uma linha do arquivo
char *le_linha_arquivo(int fd) {

    return NULL;

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
   



    close(fdOut);
    close(fdIn);

    return 0;
}

