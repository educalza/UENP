/*Fa ̧ca um programa que receba do usu ́ario um vetor de “n” n ́umeros inteiros, em seguida,
implemente uma fun ̧c ̃ao que determine e retorne o maior valor do vetor lido.*/

#include <stdio.h>

int maiorValor(int vetor[], int tamanho) {
    int maior = vetor[0]; 

    for (int i = 1; i < tamanho; i++) {
        if (vetor[i] > maior) {
            maior = vetor[i];
        }
    }

    return maior;
}

int main() {
    int n;

    printf("Digite a quantidade de numeros a ser armazenada: ");
    scanf("%d", &n);
    int vetor[n];

    printf("Digite a sequencia de numeros: \n");
    for (int i = 0; i < n; i++) {
        printf("Vetor[%d]: ", i);
        scanf("%d", &vetor[i]);
    }

    int maior = maiorValor(vetor, n);

    printf("O maior valor do vetor eh: %d\n", maior);

    return 0;
}
