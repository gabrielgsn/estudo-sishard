#include <stdio.h>
#include <stdlib.h>
#include "macros_correcoes.h"

/* Copia string e retorna uma nova string */
char *mystrcpy(char *stroriginal) {
    int n = 0;
    while (stroriginal[n] != '\0') {
        n++;
    }
    char *strcopia = (char *)malloc((n + 1) * sizeof(char));
    if (strcopia == NULL) {
        perror("malloc");
        exit(1);
    }
    for (int i = 0; i <= n; i++) {
        strcopia[i] = stroriginal[i];
    }
    return strcopia;
}

int main(int argc, char *argv[]) {
    char *strcopia;

    char *s1 = "AAAAA";
    char *t1 = mystrcpy(s1);
    assertEquals("Teste 1", strcmp(s1, t1), 0);
    assertDifferent("Teste 1 - endereço diferente", s1, t1);
    free(t1);

    s1 = "";
    t1 = mystrcpy(s1);
    assertEquals("Teste 2", strcmp(s1, t1), 0);
    assertDifferent("Teste 2 - endereço diferente", s1, t1);
    free(t1);

    s1 = "AaAa123384739jlkfjdsl";
    t1 = mystrcpy(s1);
    assertEquals("Teste 3", strcmp(s1, t1), 0);
    assertDifferent("Teste 3 - endereço diferente", s1, t1);
    free(t1);

    s1 = "Aa00asdsada";
    t1 = mystrcpy(s1);
    assertEquals("Teste 4", strcmp(s1, t1), 0);
    assertDifferent("Teste 4 - endereço diferente", s1, t1);
    free(t1);

    s1 = "0asgfd";
    t1 = mystrcpy(s1);
    assertEquals("Teste 5", strcmp(s1, t1), 0);
    assertDifferent("Teste 5 - endereço diferente", s1, t1);
    free(t1);

    printSummary

    return 0;
}

