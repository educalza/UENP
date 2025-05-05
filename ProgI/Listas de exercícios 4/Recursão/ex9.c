/*Implemente uma fun ̧c ̃ao recursiva que mostre o maior elemento de um array de inteiros. O
prot ́otipo da fun ̧c ̃ao recursiva  ́e a seguinte:
• int maiorElemento(int *vet, int tam) {...}*/

#include <stdio.h>

int maiorElemento(int *vet, int tam);

int main() {
    int tam;
    printf("Digite o tamanho do array: ");
    scanf("%d", &tam);

    int vet[tam];
    printf("Digite os elementos do array: ");
    for (int i = 0; i < tam; i++) {
        scanf("%d", &vet[i]);
    }

    int maior = maiorElemento(vet, tam);
    printf("O maior elemento do array eh: %d\n", maior);

    return 0;
}

int maiorElemento(int *vet, int tam) {

    if (tam == 1) {
        return vet[0];
    } else {

        int maxSubArray = maiorElemento(vet + 1, tam - 1);
        
        return (vet[0] > maxSubArray) ? vet[0] : maxSubArray;
    }
}