/*Implemente um programa que leia as vari ́aveis a, b e c e imprima os valores das quatros
f ́ormulas seguintes:*/

#include <stdio.h>
#include <math.h>

int main(){

    float a = 0, b = 0, c = 0;
    float formula1, formula2, formula3, formula4;

    printf("Digite o valor de a: ");
    scanf("%f", &a);

    printf("Digite o valor de b: ");
    scanf("%f", &b);

    printf("Digite o valor de c: ");
    scanf("%f", &c);

    formula1 = (a * b)/2;
    formula2 = pow(a, 2) + b + 5*c;
    formula3 = a*b*c + b + c/3 * 5 - 1;
    formula4 = pow((a*b*b), 3)/2;

    printf("Valor da primeira formula: %.2f\n", formula1);
    printf("Valor da segunda formula: %.2f\n", formula2);
    printf("Valor da terceira formula: %.2f\n", formula3);
    printf("Valor da quarta formula: %.2f\n", formula4);

    return 0;
}