#include <stdio.h>

int main(){

    float pReal, pDolar, pEuro;

    printf("Digite o preco do produto em Real:");
    scanf("%f", &pReal);

    printf("Digite o valor do Dolar atualmente:");
    scanf("%f", &pDolar);

    printf("Digite o valor do Euro atualmente:");
    scanf("%f", &pEuro);

    float conversaoRealDolar, conversaoRealEuro;

    conversaoRealDolar = pReal * pDolar;
    conversaoRealEuro = pReal * pEuro;

    printf("Valor do produto em Dolar: %.2f\n", conversaoRealDolar);
    printf("Valor do produto em Euro: %.2f", conversaoRealEuro);

    return 0;

}