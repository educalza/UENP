/*Fa ̧ca um programa que leia 50 n ́umeros inteiros e armazene em um vetor. Em seguida,
armazene os n ́umeros pares no vetor chamado par e os n ́umeros  ́ımpares no vetor  ́ımpar.
Imprima os dois vetores resultante.*/

#include <stdio.h>

int main(){

    int vetor[50];
    int contador_par = 0, contador_impar = 0;
    int par[50] = {0}, impar[50] = {0};

    printf("Digite os numeros para separar em pares e impares: \n");
    for(int i=0; i<50; i++){
        printf("Numero[%d]: ", i+1);
        scanf("%d", &vetor[i]);

        if(vetor[i]%2==0){
            par[contador_par] = vetor[i];
            contador_par++;
        }
        if(vetor[i]%2!=0){
            impar[contador_impar] = vetor[i];
            contador_impar++;

        }
    }

    for(int i=0; i<contador_par; i++){
        printf("Numeros pares: %d \n", par[i]);
    }

    printf("\n");

    for(int i=0; i<contador_impar; i++){
        printf("Numeros impares: %d \n", impar[i]);
    }
    return 0;
}