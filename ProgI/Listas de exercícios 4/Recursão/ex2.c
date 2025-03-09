/*Altere a fun ̧c ̃ao recursiva do exerc ́ıcio 1 de modo que o programa imprima todos os n ́umeros
pares de 0 at ́e n em ordem crescente.*/

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
        else{
            loop = false;
        }
    }

    printf("Numeros pares: \n");
    imprimirNumerosPares(n);

    return 0;
}

void imprimirNumerosPares(int n){

    if(n<0){
        return;
    }
    else if(n%2==0){
        imprimirNumerosPares(n-1);
        printf("%d ", n);
    }
    else{
        imprimirNumerosPares(n-1);
    }
}