/*Fa ̧ca um programa que leia uma sequˆencia de n ́umeros inteiros e armazene em um vetor.
Em seguida, implemente uma fun ̧c ̃ao que determine e mostre os n ́umeros do vetor que s ̃ao
m ́ultiplos de 7.*/

#include <stdio.h>

void multiplos(int vetor[], int tamanho) {
    printf("Numeros multiplos de 7:\n");
    for (int i = 0; i < tamanho; i++) {
        if (vetor[i] % 7 == 0) {
            printf("%d ", vetor[i]);
        }
    }
    printf("\n\n");
}

int main() {

    int n;

    printf("Digite a quantidade de numeros a ser armazenada: ");
    scanf("%d", &n);
    int vetor[n];

    printf("Digite a sequencia de numeros:\n");
    for (int i = 0; i < n; i++) {
        printf("Vetor[%d]: ", i);
        scanf("%d", &vetor[i]);
    }

    multiplos(vetor, n);

    return 0;
}
