/*Fa ̧ca uma fun ̧c ̃ao que receba um n ́umero inteiro qualquer, calcule e mostre todos os seus
divisores, bem como a quantidade de divisores existentes.*/

#include <stdio.h>

int main(){

    int num;

    printf("Digite um numero inteiro para verificar quais sao seus divisores: ");
    scanf("%d", &num);
}

int quantosDivisores(int numero, int divisores){
    int count, divisor;
    for(int i = 0; i<=numero; i++){
        divisor = numero/i;
        printf("num é dividido por: %d", divisor);
    }
}