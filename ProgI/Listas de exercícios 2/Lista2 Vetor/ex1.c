/*Fa ̧ca um programa que leia uma sequˆencia de n ́umeros qualquer e armazene em um vetor,
em seguida, implemente uma fun ̧c ̃ao que contabilize e retorne o total de ocorrˆencias do
 ́ultimo n ́umero lido desta sequˆencia.*/

#include <stdio.h>

int contarOcorrencias(int vetor[], int tamanho, int ultimoNumero) {
    int count = 0;
    for (int i = 0; i < tamanho; i++) {
        if (vetor[i] == ultimoNumero) {
            count++;
        }
    }
    return count;
}

int main() {

    int n;
    
    printf("Digite o tamanho da sequencia: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("Tamanho da sequencia invalido.\n");
        return 1;
    }

    int vetor[n];

    printf("Digite a sequencia de numeros:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &vetor[i]);
    }

    int ultimoNumero = vetor[n - 1];

    int totalOcorrencias = contarOcorrencias(vetor, n, ultimoNumero);

    printf("O ultimo numero (%d) ocorre %d vezes na sequencia.\n", ultimoNumero, totalOcorrencias);

    return 0;
}

