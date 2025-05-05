/*Implemente um programa em C que receba a base e a altura de um triˆangulo e imprima a
sua  ́area.*/     /*area do triangulo = base*altura/2 */

#include <stdio.h>

int main(){

    float base, altura, area;
    printf("Digite o valor da base de seu triangulo isosceles: ");
    scanf("%f", &base);

    printf("Digite o valor da altura de seu triangulo isosceles: ");
    scanf("%f", &altura);

    area = (base * altura)/2;

    printf("Valor da area de seu triangulo isosceles: %.2f", area);

    return 0;
}