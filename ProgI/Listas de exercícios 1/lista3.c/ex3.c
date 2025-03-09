/*Fa ̧ca um programa que receba como entrada um n ́umero inteiro n e exiba o resultado da
seguinte soma:*/

#include <stdio.h>

int main() {
    int num;
    float soma = 0.0;  // Inicialize a soma com 0.0

    printf("Digite um numero para realizar a soma: ");
    scanf("%d", &num);

    for (int i = 1; i <= num; i++) {
        soma = soma + (1.0 / i);
    }

    printf("Resultado da soma: %f\n", soma);

    return 0;
}
