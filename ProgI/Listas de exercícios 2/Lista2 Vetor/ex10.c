/*Fa ̧ca um programa que receba um vetor de 20 elementos e, em seguida, efetue a troca dos
10 primeiros elementos pelos 10  ́ultimos, exibindo ao final o vetor resultante.*/

#include <stdio.h>

int main() {

    int vetor[20];

    printf("Digite 20 numeros para o vetor:\n");
    for (int i = 0; i < 20; i++) {
        printf("Elemento [%d]: ", i + 1);
        scanf("%d", &vetor[i]);
    }

    for (int i = 0; i < 10; i++) {
        int temp = vetor[i];
        vetor[i] = vetor[i + 10];
        vetor[i + 10] = temp;
    }

    printf("Vetor depois da troca:\n");
    for (int i = 0; i < 20; i++) {
        printf("%d ", vetor[i]);
    }

    return 0;
}
