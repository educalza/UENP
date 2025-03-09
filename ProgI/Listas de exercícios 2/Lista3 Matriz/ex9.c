/*9. Fa ̧ca um programa que receba as dimens ̃oes e os elementos de uma matriz, gere e exiba a
sua transposta.*/

#include <stdio.h>

int main() {
    
    int linha, coluna;

    printf("Digite o numero de linhas da matriz: ");
    scanf("%d", &linha);

    printf("Digite o numero de colunas da matriz: ");
    scanf("%d", &coluna);

    int matriz[linha][coluna];
    int transposta[coluna][linha];

    printf("Digite os numeros da matriz:\n");
    for (int i = 0; i < linha; i++) {
        for (int j = 0; j < coluna; j++) {
            scanf("%d", &matriz[i][j]);
            transposta[j][i] = matriz[i][j]; 
        }
    }
    printf("\n");

    printf("Matriz Transposta:\n");
    for (int i = 0; i < coluna; i++) {
        for (int j = 0; j < linha; j++) {
            printf("%d ", transposta[i][j]);
        }
        printf("\n");
    }

    return 0;
}