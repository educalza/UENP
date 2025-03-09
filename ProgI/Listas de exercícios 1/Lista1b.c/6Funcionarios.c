/*Implemente um programa em C que leia o n ́umero de um funcion ́ario, seu n ́umero de horas
trabalhadas, o valor que recebe por hora e calcula o sal ́ario desse funcion ́ario. A seguir,
mostre o n ́umero e o sal ́ario do funcion ́ario.*/

#include <stdio.h>

int main (){

    int funcionario, horasTrabalhadas;
    float ValorPorHora, salario;

    printf("Digite o numero do funcionario: ");
    scanf("%d", &funcionario);

    printf("Digite as horas trabalhada por ele: ");
    scanf("%d", &horasTrabalhadas);

    printf("Digite o valor que o mesmo recebe por hora: ");
    scanf("%f", &ValorPorHora);

    salario = (horasTrabalhadas * ValorPorHora) * 30;

    printf("O valor do salario do funcionario %d eh igual a %.2f: ", funcionario, salario);

    return 0;
}