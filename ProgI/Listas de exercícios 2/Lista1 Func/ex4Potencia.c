/*Fa ̧ca um programa que calcule a potencia de um n ́umero qualquer fornecido pelo usu ́ario.
Para isso, implemente uma fun ̧c ̃ao que receba como parˆametro dois n ́umeros inteiros x e n,
calcule e retorne para o programa principal o resultado de x
n. NAO USE nenhuma fun ̧c ̃ao  ̃pronta (ou seja, pow()).*/

#include <stdio.h>

double calcularPotencia(double x, int n) {
    double resultado = 1.0;
    for (int i = 0; i < n; i++) {
        resultado *= x;
    }
    return resultado;
}

int main() {
    double base;
    int expoente;

    printf("Digite a base: ");
    scanf("%lf", &base);
    printf("Digite o expoente: ");
    scanf("%d", &expoente);

    double resultado = calcularPotencia(base, expoente);
    printf("%.2lf^%d = %.2lf\n", base, expoente, resultado);

    return 0;
}
