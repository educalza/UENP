/*Fa ̧ca um programa que leia uma matriz de ordem 3 x 5 e calcule a soma dos elementos de
cada coluna da matriz, armazenando os resultados em um vetor. Depois, o programa deve
exibir o vetor resultante.*/
#define LIN 3
#define COL 5

#include <stdio.h>

int main(){

    int matriz[LIN][COL];
    int somaCol[5] = {0};

    printf("Digite os numeros da matriz 3x5: \n");
    for(int i=0; i<LIN; i++){
        for(int j=0; j<COL; j++){
            printf("Matriz[%d][%d]: ", i, j);
            scanf("%d", &matriz[i][j]);
        }
    }
    for(int j=0; j<COL; j++){
        for(int i=0; i<LIN; i++){
            somaCol[j] += matriz[i][j];
        }
    }
    for(int j=0; j<COL; j++){
        printf("Coluna[%d]: %d\n", j, somaCol[j]);
    }
    return 0;
}