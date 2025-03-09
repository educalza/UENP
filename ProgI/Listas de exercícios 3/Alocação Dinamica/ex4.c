/*Implemente um programa que leia duas matrizes de inteiros m1 e m2 de ordem M x N e
preencha uma terceira matriz soma que  ́e resultado da soma de m1 e m2. As matrizes devem
ser alocadas dinamicamente. N ̃ao esque ̧ca de verificar se  ́e poss ́ıvel fazer a aloca ̧c ̃ao de
mem ́oria.*/

#include <stdio.h>
#include <stdlib.h>

void registrarMatriz(int **m1, int **m2, int M, int N);
void somarM1eM2(int **m1, int **m2, int M, int N);

int main(){

    int M, N;

    printf("Digite os tamanhos das matrizes\n");
    printf("M: ");
    scanf("%d", &M);
    printf("N: ");
    scanf("%d", &N);

    int **m1 = (int**)calloc(M, sizeof(int*));
    for(int i=0; i<M; i++){
        m1[i] = (int*)calloc(N, sizeof(int));
    }

    int **m2 = (int**)calloc(M, sizeof(int*));
    for(int i=0; i<M; i++){
        m2[i] = (int*)calloc(N, sizeof(int));
    }

    registrarMatriz(m1, m2, M, N);
    printf("\n");
    somarM1eM2(m1, m2, M, N);

    for(int i=0; i<M; i++){
        free(m1[i]);
    }
    free(m1);
    for(int i=0; i<M; i++){
        free(m2);
    }
    free(m2);
}

void registrarMatriz(int **m1, int **m2, int M, int N){

    printf("Digite os numeros das matrizes m1 e m2 \n\n");
    printf("M1.\n");
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            printf("M1[%d][%d]: ", i, j);
            scanf("%d", &m1[i][j]);
        }
    }
    printf("\n");
    printf("M2.\n");
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            printf("M2[%d][%d]: ", i, j);
            scanf("%d", &m2[i][j]);
        }
    }

}

void somarM1eM2(int **m1, int **m2, int M, int N){

    int **Msoma = (int **)calloc(M, sizeof(int*));
    for(int i=0; i<M; i++){
        Msoma[i] = (int*)calloc(N, sizeof(int*));
    }

    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            Msoma[i][j] = (m1[i][j]) + (m2[i][j]);
        }
    }
    printf("Soma das matrizes: ");
    for(int i=0; i<M; i++){
        for(int j=0; j<N; j++){
            printf("%d ", Msoma[i][j]);
        }
    }
    free(Msoma);
}