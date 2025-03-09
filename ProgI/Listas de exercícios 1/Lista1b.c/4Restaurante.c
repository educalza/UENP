/*Implemente um programa em C que leia um valor representando o gasto realizado por um
cliente do restaurante e imprima o valor total a ser pago, considerando os 10% do gar ̧com.*/

#include <stdio.h>

int main (){

    float gasto, garcom = 10, valorComGorjeta, valorTotal;

    printf("Valor gasto pelo cliente: ", &gasto);
    scanf("%f", &gasto);

    valorComGorjeta = (gasto*garcom)/100;
    valorTotal = gasto + valorComGorjeta;

    printf("Valor a ser pago com gorjeta: %.2f", valorTotal);

    return 0;
}