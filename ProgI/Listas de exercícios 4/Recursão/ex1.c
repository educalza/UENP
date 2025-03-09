/*Implemente uma fun ̧c ̃ao recursiva que receba um n ́umero inteiro positivo par n e imprima
todos os n ́umeros pares de 0 at ́e n em ordem decrescente.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void imprimirNumerosPares(int n);

int main(){

    int n;
    bool loop = true;

    printf("Digite um numero inteiro positivo par: ");
    while(loop){
        scanf("%d", &n);
        if(n<=0){
            printf("Numero invalido, digite outro numero: ");
        }
        if(n>1){
            loop = false;
        }
    }

    printf("Numeros pares: \n");
    if(n==1){
        printf("0");
        exit(0);
    }
    imprimirNumerosPares(n);

    return 0;
}

void imprimirNumerosPares(int n){

    if(n==1){
        printf("0");
        exit(0);
    }
    else if(n%2==0){
        printf("%d ", n);
    }
        imprimirNumerosPares(n-1);
}