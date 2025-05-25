#include <stdio.h>


int ex4_solucao(long rdi) {
    int eax, edx;
    scanf("%d %d", &eax, &edx);
    eax = eax+2*edx;
    eax += rdi;
    return eax;
}