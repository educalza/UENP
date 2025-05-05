#include <stdio.h>

#define TAM_ENTRADA 8

int main() {
    char w[TAM_ENTRADA] = "abb#aba\0";
    int i = 0, j = TAM_ENTRADA - 2;

    printf("Monolítico\n");

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
