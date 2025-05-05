/*Fa ̧ca um programa que leia o nome e o valor de determinada mercadoria de uma loja.
Sabendo que o desconto para pagamento `a vista  ́e de 10% sobre o valor total, calcule o
valor a ser pago `a vista. Escreva o nome da mercadoria, o valor total, o valor do desconto
e o valor a ser pago `a vista.*/

#include <stdio.h>
#define MAX 100

int main(){

    char Nome[MAX];
    float ValorTotal;

    printf("Insira o nome do produto:\n");
    fgets(Nome, MAX, stdin);

    printf("Insira o Valor:\n");
    scanf("%f", &ValorTotal);

    printf("Voce deseja comprar o %s?\n", Nome);
    printf("Valor total do produto: R$ %.2f\n", ValorTotal);
    printf("R$ %.2f de desconto pago a vista: R$ %.2f\n", ValorTotal*0.1, ValorTotal*0.9);

return 0;
}