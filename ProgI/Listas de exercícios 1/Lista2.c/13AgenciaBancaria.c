#include <stdio.h>

int main() {
    float saldoMedio, valorCredito;

    printf("Digite o saldo médio do cliente: ");
    scanf("%f", &saldoMedio);

    if (saldoMedio > 400) {
        valorCredito = saldoMedio * 0.3;
    } else if (saldoMedio > 300) {
        valorCredito = saldoMedio * 0.25;
    } else if (saldoMedio > 200) {
        valorCredito = saldoMedio * 0.2;
    } else {
        valorCredito = saldoMedio * 0.1;
    }

    printf("O valor do crédito é: R$ %.2f\n", valorCredito);

    return 0;
} 