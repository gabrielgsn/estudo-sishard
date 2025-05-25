#include <stdio.h>
#include <stdlib.h>
#include "macros_correcoes.h"

int tamanho(char *str) {
    int n = 0;
    while (str[n] != '\0') {
        n++;
    }
    return n;
}

/* Recebe duas strings e concatena em uma terceira */
char *mystrcat(char *str1, char *str2) {
    int n1 = tamanho(str1);
    int n2 = tamanho(str2);
    char *str3 = (char *)malloc((n1 + n2 + 1) * sizeof(char));
    if (str3 == NULL) {
        perror("malloc");
        exit(1);
    }
    for (int i = 0; i < n1; i++) {
        str3[i] = str1[i];
    }
    for (int i = 0; i < n2; i++) {
        str3[n1 + i] = str2[i];
    }
    str3[n1 + n2] = '\0'; // Adiciona o caractere nulo no final
    return str3;
}

int main(int argc, char *argv[]) {
    # define teste1 "str1"
    # define teste2 "str2"
    # define teste3 "0 str 3"


    char *res = mystrcat(teste1, teste2);
    assertEquals("teste 1", strcmp(res, teste1 teste2), 0);
    free(res);

    res = mystrcat(teste1, teste1);
    assertEquals("teste 2", strcmp(res, teste1 teste1), 0);
    free(res);

    res = mystrcat(teste1, teste3);
    assertEquals("teste 3", strcmp(res, teste1 teste3), 0);
    free(res);

    res = mystrcat(teste3, teste1);
    assertEquals("teste 4", strcmp(res, teste3 teste1), 0);
    free(res);

    res = mystrcat("", teste2);
    assertEquals("teste vazio 1", strcmp(res, teste2), 0);
    free(res);

    res = mystrcat(teste1, "");
    assertEquals("teste vazio 2", strcmp(res, teste1), 0);
    free(res);

    res = mystrcat("", "");
    assertEquals("teste vazio 3", strcmp(res, ""), 0);
    free(res);

    printSummary

    return 0;
}
