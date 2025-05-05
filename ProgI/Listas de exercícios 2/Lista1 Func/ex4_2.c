#include <stdio.h>

double calcularPotencia(double x, int n); // prototipo

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

double calcularPotencia(double x, int n) {
    double resultado = 1.0;
    for (int i = 0; i < n; i++) {
        resultado *= x;
    }
    return resultado;
}
