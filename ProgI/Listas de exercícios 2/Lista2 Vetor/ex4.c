/*Fa ̧ca um programa que leia uma sequˆencia de 21 n ́umeros inteiros e armazene em um vetor.
Depois, determine se o  ́ultimo n ́umero lido est ́a presente nos 20 primeiros e mostre as
posi ̧c ̃oes do vetor em que ele se encontra.*/

#include <stdio.h>

int main(){

    int vetor[21];
    int count = 0;

    for(int i=0; i<21; i++){
        printf("Vetor[%d]: ", i+1);
        scanf("%d", &vetor[i]);
    }

    for(int i=0; i<21; i++){
        if(vetor[i] == vetor[20]){
            count++;
        }
    }

    printf("O numero %d apareceu %d vezes, nos respectivos vetores: \n\n", vetor[20], count);
    for(int i=0; i<21; i++){
        if(vetor[i] == vetor[20]){
            printf("Vetor[%d] ", i);
        }
    }
    printf("\n");

    return 0;
}