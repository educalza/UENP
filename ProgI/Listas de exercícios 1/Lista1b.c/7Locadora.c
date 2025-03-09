/*Implemente um programa em C que leia quantos dias a pessoa ficou com o carro: [1; 30]
e quantos kilˆometros ela rodou [1; 1000], calcule e mostre o valor total que a pessoa deve
pagar pelo aluguel do carro.*/

#include <stdio.h>

int main (){

    float diaria = 30, Km = 0.01, desconto = 10, dias, KmRodado;
    float ValorDasDiarias, ValorDescontado, ValorTotal, ValorComKm, ValorAluguel;

    printf("Digite a quantidade de dias que o cliente ficou com o carro: ");
    scanf("%f", &dias);

    printf("Digite a quilometragem dirigida pelo cliente: ");
    scanf("%f", &KmRodado);

    ValorDasDiarias = diaria * dias; // Valor das diarias
    ValorComKm = KmRodado*Km; // Valor por quilometro rodado
    ValorAluguel = ValorDasDiarias + ValorComKm; // Valor das diarias junto com km

    ValorDescontado = (ValorAluguel*desconto)/100; // retirada de 10%
    ValorTotal = ValorAluguel - ValorDescontado; // diferença entre valot total e desconto

    printf("Valor total descontado que o cliente precisa pagar: %.2f", ValorTotal);

    return 0;
}