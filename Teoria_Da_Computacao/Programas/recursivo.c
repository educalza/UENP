#include <stdio.h>

#define TAM_ENTRADA 8

int verificaSimetriaPalavra(const char palavra[TAM_ENTRADA], int pontoInicial, int pontoFinal) {
    if (pontoInicial >= pontoFinal) 
        return 1;
    if (palavra[pontoInicial] != palavra[pontoFinal]) 
        return 0;
    return verificaSimetriaPalavra(palavra, pontoInicial + 1, pontoFinal - 1);
}

int main() {
    char w[TAM_ENTRADA] = "aba#bba\0";
    int tamanho = TAM_ENTRADA - 2;

    printf("Recursivo\n");

    if (tamanho % 2 != 0) {
        printf("FALSE");
        return 0;
    }

    if (verificaSimetriaPalavra(w, 0, tamanho - 1))
        printf("TRUE");
    else
        printf("FALSE");

    return 0;
}
