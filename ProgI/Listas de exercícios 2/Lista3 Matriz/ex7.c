/*7. Fa ̧ca um programa que leia uma matriz quadrada com valores inteiros com dimens ̃ao
definida pelo usu ́ario, leia uma constante x e, atrav ́es de uma fun ̧c ̃ao, multiplique os
elementos da diagonal secund ́aria da matriz com essa constante. Ao final, o programa
deve exibir a matriz resultante.*/
#include <stdio.h>
#define N 100

void multiplicaDiagonal(int matriz[][N], int dimensao, int constante) {
    for (int i = 0; i < dimensao; i++) {
        matriz[i][dimensao - i - 1] *= constante;
    }
}

int main() {

    int dimensao;
    int constante;

    printf("Digite a dimensao da matriz quadrada: ");
    scanf("%d", &dimensao);

    int matriz[N][N];

    printf("Digite os elementos da matriz:\n");
    for (int i = 0; i < dimensao; i++) {
        for (int j = 0; j < dimensao; j++) {
            scanf("%d", &matriz[i][j]);
        }
    }

    printf("Digite a constante: ");
    scanf("%d", &constante);

    multiplicaDiagonal(matriz, dimensao, constante);

    printf("\n");
    printf("Matriz resultante:\n");
    for (int i = 0; i < dimensao; i++) {
        for (int j = 0; j < dimensao; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }

    return 0;
}