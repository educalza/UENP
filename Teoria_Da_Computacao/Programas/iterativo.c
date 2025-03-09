#include <stdio.h>

#define TAM_ENTRADA 8

int main() {
    char w[TAM_ENTRADA] = "aba#bba\0";
    int i = 0, j = TAM_ENTRADA - 2;

    printf("Iterativo\n");

    // Verifica se o número total de caracteres (exceto o terminador nulo) é ímpar
    if ((j + 1) % 2 != 0) {
        printf("FALSE");
        return 0;
    }

    while (i < j) {
        if (w[i] != w[j]) {
            printf("FALSE");
            return 0;
        }
        i++;
        j--;
    }

    printf("TRUE");
    return 0;
}
