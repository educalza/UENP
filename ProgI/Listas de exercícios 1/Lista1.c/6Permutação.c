// ler valores de x e y e depois permutalos
#include <stdio.h>

int main(){

    float x = 0, y = 0, temp;

    printf("Valor de x para que seja permutado:");
    scanf("%f", &x);

    printf("Valor de y para que seja permutado:");
    scanf("%f", &y);

    temp = x;    // temp é uma variável auxilar, na qual armazena um valor temporariamente
    x = y;
    y = temp;

    printf("Valores de x e y permutados: x = %.2f, y = %.2f\n", x, y);

    return 0; 
}