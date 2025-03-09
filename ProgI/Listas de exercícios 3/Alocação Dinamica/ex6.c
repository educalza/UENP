/*Implemente uma fun ̧c ̃ao que receba como parˆametro um valor n e retorne o ponteiro para
uma matriz alocada dinamicamente contendo n linhas e n colunas. Preencha a matriz com
o valor 1 na diagonal secund ́aria e 0 nas demais posi ̧c ̃oes.*/

#include <stdio.h>
#include <stdlib.h>

int **DiagonalMatriz(int n);

int main(){

    int n;
    printf("Digite o valor da coluna e linha da matriz: ");
    scanf("%d", &n);

    int **funcao = DiagonalMatriz(n);

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            printf("%d ", funcao[i][j]);
        }
        printf("\n");
    }
    for(int i=0; i<n; i++){
        free(funcao[i]);
    }
    free(funcao);
}

int **DiagonalMatriz(int n){

    int **matriz = (int**)calloc(n, sizeof(int*));
    for(int i=0; i<n; i++){
        matriz[i] = (int*)calloc(n, sizeof(int));
    }

    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            matriz[i][j] = (i + j == n-1) ? 1 : 0;
        }
    }
    return matriz;
}