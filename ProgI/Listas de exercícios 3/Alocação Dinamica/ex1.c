/*Implemente uma fun ̧c ̃ao que receba como parˆametro dois vetores vetX e vetY de tamanho n.
A fun ̧c ̃ao deve retornar o ponteiro para um vetor soma de tamanho n alocado dinamicamente,
em que soma = vetX + vetY.*/

#include <stdio.h>
#include <stdlib.h>

int *RetornaPonteiro(int vetX[], int vetY[], int n);

int main(){

    int n=0;

    printf("Digite o tamanho dos vetores:");
    scanf("%d", &n);

    int vetX[n], vetY[n];
    int *funcao;

    printf("Digite o valor de x e y.\n");
    for(int i=0; i<n; i++){
        printf("X: ");
        scanf("%d", &vetX[i]);
        printf("Y: ");
        scanf("%d", &vetY[i]);
    }
    
    funcao = RetornaPonteiro(vetX, vetY, n);

    printf("Soma: ");
    for(int i=0; i<n; i++){
        printf("%d ", funcao[i]);
    }

    free(funcao);

    return 0;
}

int *RetornaPonteiro(int vetX[], int vetY[], int n){

    int *soma = (int*)malloc(n * sizeof(int));

    if(soma == NULL){
        printf("Deu erro.\n");
        exit(1);
    }

    for(int i=0; i<n; i++){
        soma[i] = vetX[i] + vetY[i];
    }
    
    return soma;
}