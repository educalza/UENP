/*Fa ̧ca um programa que receba como entrada 3 numeros reais maiores que zero correspondendo
ao comprimento dos lados do triˆangulo, em seguida, classifique-o em:*/

#include <stdio.h>

int main(){

    double lado1, lado2, lado3;

    printf("Digite os valores do triangulos maiores que zero:\n");
    printf("lado 1: ");
    scanf("%lf", &lado1);

    printf("lado 2: ");
    scanf("%lf", &lado2);

    printf("lado 3: ");
    scanf("%lf", &lado3);

    if(lado1 == lado2 && lado1 == lado3 && lado2 == lado3){
        printf("Triangulo equilatero");
    } else if(lado1 == lado2 || lado1 == lado3 || lado2 == lado3){
        printf("Triangulo isosceles");
    } else if(lado1 != lado2 && lado1 != lado3 && lado2 != lado3){
        printf("Triangulo escaleno");
    }
    return 0;   
}