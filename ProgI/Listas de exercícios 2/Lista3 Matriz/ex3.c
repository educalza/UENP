/*Fa ̧ca um programa que receba uma matriz de inteiros de ordem 5 x 3 e verifique quais os
elementos da matriz s ̃ao m ́ultiplos de 3, armazenando esses elementos em um vetor. Depois,
o programa deve exibir o vetor resultante.*/

#define LIN 5
#define COL 3

#include <stdio.h>

int main(){

    int mat[LIN][COL] = {0};
    int vetor[15] = {0};
    int countMulti = 0;

    for(int i=0; i<LIN; i++){
        for(int j=0; j<COL; j++){
            printf("Matriz[%d][%d]: ", i, j);
            scanf("%d", &mat[i][j]);

            if(mat[i][j]%3==0){
                vetor[countMulti] = mat[i][j];
                countMulti++;
            }
        }
    }

    printf("Numeros multiplos de 3: \n");
    for(int i=0; i<countMulti; i++){
            printf("%d ", vetor[i]);
    }
    return 0;
}