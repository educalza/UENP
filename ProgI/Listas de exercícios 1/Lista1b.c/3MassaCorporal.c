/*Implemente um programa em C que, dada a massa e altura de uma pessoa, imprima o seu
IMC.*/

#include <stdio.h>
#include <math.h>

int main(){

    float altura, peso, valorIMC;

    printf("Digite sua altura: ");
    scanf("%f", &altura);

    printf("Digite seu peso: ");
    scanf("%f", &peso);

    valorIMC = peso/pow(altura, 2);

    printf("Valor de seu IMC: %.2f", valorIMC);

    return 0;
}