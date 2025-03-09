/*Na semana do dia do trabalhador um resort resolveu dar a seus clientes um desconto de
30% na diaria. O resort, que conta com 45 acomodacoes, almeja dobrar a sua taxa de
ocupacao que hoje  ́e de 35%. Faca um programa que receba a valor normal da diaria,
calcule e mostre, 1) o valor da diaria com desconto, 2) o total arrecadado com o dobro
de ocupacao e diaria com desconto e 3) o total arrecadado com 35% de ocupacao e diaria
normal.*/
#include <stdio.h>

int main(){

    float valorDiaria, desconto = 30, valorDescontado, valorComDesconto;

    printf("Digite o valor da diaria: ");
    scanf("%f", &valorDiaria);

    valorDescontado = (valorDiaria * desconto)/100;
    valorComDesconto = valorDiaria - valorDescontado;

    printf("Valor da diaria dobrada com desconto aplicado: %.2f\n", valorComDesconto);

    float acomodacoes = 45, ocupacao1, ocupacao2, totalComDesconto;
    ocupacao1 = (acomodacoes * 35)/100;
    ocupacao2 = (acomodacoes * (35 * 2))/100;
    totalComDesconto = ocupacao2 * valorComDesconto;
    
    printf("Valor total arrecadado com desconto: %.2f\n", totalComDesconto);

    float TotalSemDesconto = ocupacao1 * valorDiaria;

    printf("Valor arrecadado com 35 por cento de ocupacao e sem desconto: %.2f", TotalSemDesconto);

    return 0;
}