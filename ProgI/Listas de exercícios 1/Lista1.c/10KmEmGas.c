/*Faca um programa que receba a distancia total percorrida em km e o total de combustıvel
gasto em litros, calcule e mostre o consumo medio de um veıculo.*/

#include <stdio.h>

int main(){

    float km, gasolina, gastoPorKm;

    printf("Digite a distancia percorrida em Km: ");
    scanf("%f", &km);

    printf("Digite a quantidade de litros de gasolina gastos na distancia percorrida: ");
    scanf("%f", &gasolina);

    gastoPorKm = km / gasolina;

    printf("Consumo medio de gasolina do seu veiculo: %.2f Km/L", gastoPorKm);

    return 0;
}