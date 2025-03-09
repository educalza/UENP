/*8. Fa ̧ca um programa que leia uma matriz quadrada de tamanho qualquer, calcule e mostre
por meio de uma fun ̧c ̃ao, a soma dos elementos que n ̃ao pertencem a nenhuma das diagonais
– principal e secund ́aria.*/

#include <stdio.h>
#define N 100

int somaMENOSDiagonais(int matriz[][N], int dimensao) {
    int soma = 0;

    for (int i = 0; i < dimensao; i++) {
        for (int j = 0; j < dimensao; j++) {
            if (i != j && i != dimensao - j - 1) {
                soma += matriz[i][j];
            }
        }
    }

    return soma;
}

int main() {
    int dimensao;

    printf("Digite a dimensao da matriz quadrada: ");
    scanf("%d", &dimensao);

    int matriz[N][N];

    printf("Digite os elementos da matriz:\n");
    for (int i = 0; i < dimensao; i++) {
        for (int j = 0; j < dimensao; j++) {
            scanf("%d", &matriz[i][j]);
        }
    }
    printf("\n");
    int soma = somaMENOSDiagonais(matriz, dimensao);

    
    printf("Soma dos elementos fora das diagonais: %d\n", soma);

    return 0;
}