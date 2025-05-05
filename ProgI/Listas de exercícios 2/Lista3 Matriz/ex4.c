/*Fa ̧ca um programa que leia uma matriz 4 x 5, determine e mostre atrav ́es de uma fun ̧c ̃ao
o menor elemento dessa matriz.*/

#include <stdio.h>
#define L 4
#define C 5

int MenorNumero(int matriz[L][C]) {
    int menor = matriz[0][0];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            if (matriz[i][j] < menor) {
                menor = matriz[i][j];
            }
        }
    }

    return menor;
}

int main() {

    int matriz[L][C];

    printf("Digite os numeros da matriz 4x5:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            printf("Matriz[%d][%d]: ", i, j);
            scanf("%d", &matriz[i][j]);
        }
    }

    int menor = MenorNumero(matriz);

    printf("O menor elemento da matriz eh: %d\n", menor);

    return 0;
}
