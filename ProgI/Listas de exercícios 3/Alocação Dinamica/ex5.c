/*Implemente um programa que leia uma matriz de inteiros alocada dinamicamente com
dimens ̃oes definidas pelo usu ́ario. Em seguida, implemente uma fun ̧c ̃ao respons ́avel por
buscar um n ́umero n na matriz. A fun ̧c ̃ao deve retornar 1, caso encontre n na matriz ou 0,
caso contr ́ario. O prot ́otipo da fun ̧c ̃ao  ́e a seguinte:
int buscaNumero(int numero, int **array, int linha, int coluna) {..}*/

#include <stdio.h>
#include <stdlib.h>

int buscaNumero(int numero, int **array, int linha, int coluna);

int main(){

    int L, C;

    printf("Digite o tamanho das matrizes.\n");
    printf("LINHA: ");
    scanf("%d", &L);
    printf("COLUNA: ");
    scanf("%d", &C);

    int **matriz = (int**)calloc(L, sizeof(int*));
    for(int i=0; i<L; i++){
        matriz[i] = (int*)calloc(C, sizeof(int*));
    }

    printf("Digite os numeros da matriz.\n");
    for(int i=0; i<L; i++){
        for(int j=0; j<C; j++){
            printf("Matriz[%d][%d]: ", i, j);
            scanf("%d", &matriz[i][j]);
        }
    }
    printf("\n");
    int n=0;
    printf("Digite um numero n para buscar na matriz: ");
    scanf("%d", &n);

    int funcao=0;
    funcao = buscaNumero(n, matriz, L, C);
    if(funcao == 1){
        printf("O numero %d apareceu na matriz.\n", n);
    }
    if(funcao == 0){
        printf("O numero %d nao apareceu na matriz.\n", n);
    }

    free(matriz);
    return 0;
}

int buscaNumero(int numero/*n*/, int **array/*matriz*/, int linha/*L*/, int coluna/*C*/){

    for(int i=0; i<linha; i++){
        for(int j=0; j<coluna; j++){
            if(array[i][j]==numero){
                return 1;
                break;
            }
        }
    }
    for(int i=0; i<linha; i++){
        for(int j=0; j<coluna; j++){
            if(array[i][j]!=numero){
                return 0;
            }
        }
    }
}