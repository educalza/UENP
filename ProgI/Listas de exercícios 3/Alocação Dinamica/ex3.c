/*Implemente um programa que leia uma matriz de inteiros alocada dinamicamente, em que
o n ́umero de linhas e o n ́umero de colunas s ̃ao determinados pelo usu ́ario. Verifique quais
elementos da matriz s ̃ao  ́ımpares e m ́ultiplos de 7. Na sequˆencia, armazene os elementos
identificados em vetor, tamb ́em alocado dinamicamente, e mostre-os na tela.*/

#include <stdio.h>
#include <stdlib.h>

void elementosImpares(int **matriz, int L, int C);
void elementosMultiplosde7(int **matriz, int L, int C);

int main(){

    int C, L;

    printf("Digite a quantidade de Linha da matriz: ");
    scanf("%d", &L);
    printf("Digite a quantidade de coluna da matriz: ");
    scanf("%d", &C);

    int **matriz = (int **)calloc(L, sizeof(int*));

    for(int i=0; i<L; i++){
        matriz[i] = (int *)calloc(C, sizeof(int));
    }

    printf("Digite os valores da matriz:\n");
    for(int i=0; i<L; i++){
        for(int j=0; j<C; j++){
            printf("Matriz[%d][%d]: ", i, j);
            scanf("%d", &matriz[i][j]);
        }
    }

    printf("Elementos impares: ");
    elementosImpares(matriz, L, C);
    printf("\n");
    printf("Elementos multiplos de 7: ");
    elementosMultiplosde7(matriz, L, C);

    for(int i=0; i<L; i++){
        free(matriz[i]);
    }
    free(matriz);

    return 0;
}

void elementosImpares(int **matriz, int L, int C){

    int count = 0;
    int *nImpares;

    for(int i=0; i<L; i++){
        for(int j=0; j<C; j++){
            if(matriz[i][j]%2!=0){
                count++;
            }
        }
    }
    nImpares = (int *)calloc(count, sizeof(int));
    int k=0;
    for(int i=0; i<L; i++){
        for(int j=0; j<C; j++){
            if(matriz[i][j]%2!=0){
                nImpares[k] = matriz[i][j];
                k++;
            }
        }
    }
    for(int i=0; i<count; i++){
        printf("%d ", nImpares[i]);
    }
    free(nImpares);
}

void elementosMultiplosde7(int **matriz, int L, int C){
    
    int count=0;
    int *nMultiplos7;

    for(int i=0; i<L; i++){
        for(int j=0; j<C; j++){
            if(matriz[i][j]%7==0){
                count++;
            }
        }
    }
    nMultiplos7 = (int *)calloc(count,sizeof(int));
    int k=0;
    for(int i=0; i<L; i++){
        for(int j=0; j<C; j++){
            if(matriz[i][j]%7==0){
                nMultiplos7[k] = matriz[i][j];
                k++;
            }
        }
    }

    for(int i=0; i<count; i++){
        printf("%d ", nMultiplos7[i]);
    }
    free(nMultiplos7);
}