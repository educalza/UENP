/*Fa ̧ca um programa que leia uma matriz de ordem 5 x 3, calcule e mostre a soma de cada
linha da matriz.*/

#define LIN 5
#define COL 3

#include <stdio.h>

int main(){

    int mat[LIN][COL], l/*(linha)5*/, c/*(coluna)3*/;
    int soma[LIN] = {0};

    printf("Digite as linhas da matriz: \n");

    for(l=0; l<LIN; l++){
        for(c=0; c<COL; c++){
            printf("Matriz[%d][%d]: ", l, c);
            scanf("%d", &mat[l][c]);
            soma[l] += mat[l][c];
        }
    }
    for(l=0; l<LIN; l++){
        printf("Soma da linha[%d]: %d\n", l, soma[l]);
    }
    return 0;
}