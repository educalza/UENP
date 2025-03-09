/*Fa ̧ca um programa que leia uma matriz de ordem 5 x 5, calcule e mostre a m ́edia dos
elementos da diagonal principal.*/

#include <stdio.h>
#define L 5
#define C 5

int main() {

    int matriz[L][C];
    double somaDiagonal = 0.0;

    printf("Digite os nuemros da matriz:\n");

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("Matriz[%d][%d]: ", i, j);
            scanf("%d", &matriz[i][j]);
        }
    }

    for (int i = 0; i < 5; i++) {
        somaDiagonal += matriz[i][i];
    }

    double mediaDiagonal = somaDiagonal / 5;

    printf("Média dos elementos da diagonal principal: %.2lf\n", mediaDiagonal);

    return 0;
}
