/*Implemente uma fun ̧c ̃ao que receba um vetor de inteiros, o tamanho do vetor e um inteiro
pos passado por referˆencia. A fun ̧c ̃ao retorna o maior elemento do vetor e, na vari ́avel pos,
a posi ̧c ̃ao do maior elemento do vetor.*/

#include <stdio.h>

    int encontrarMaiorElemento(const int vetor[], int tamanho, int *pos);

int main() {

    int vetor[] = {10, 5, 8, 20, 15};
    int tamanho = sizeof(vetor) / sizeof(vetor[0]);

    int posicaoMaior;
    int maiorElemento = encontrarMaiorElemento(vetor, tamanho, &posicaoMaior);

    if (maiorElemento != -1) {
        printf("Maior elemento: %d\n", maiorElemento);
        printf("Posicao do maior elemento: %d\n", posicaoMaior);
    } else {
        printf("O vetor esta vazio.\n");
    }

    return 0;
}

int encontrarMaiorElemento(const int vetor[], int tamanho, int *pos){

    int maior = vetor[0];
    *pos = 0;

    for (int i = 1; i < tamanho; i++) {
        if (vetor[i] > maior) {
            maior = vetor[i];
            *pos = i;
        }
    }

    return maior;
}