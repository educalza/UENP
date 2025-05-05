/*Faca um algoritmo que recebe o salario-base de um funcionario, calcule e mostre seu salario
a receber, sabendo-se que ele tem gratificacao de R$ 50,00 e paga imposto de 12% sobre o
sal ́ario-base.*/

#include <stdio.h>

int main(){

    float salarioB, sImposto, sGratificacao = 50, final;

    printf("Digite o seu salario-base:");
    scanf("%f", &salarioB);

    sImposto = salarioB*0.12;

    final = salarioB + sGratificacao - sImposto;

    printf("Seu salario final:%.2f", final);

    return 0;
}