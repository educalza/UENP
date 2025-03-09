/*Fa ̧ca um programa que receba o
sal ́ario atual, o cargo e calcule o reajuste do sal ́ario. O programa deve mostrar o valor do
reajuste e o sal ́ario j ́a atualizado.*/

#include <stdio.h>

int main(){

    float salarioAtual, cargo, salarioNovo;
    float reajuste1 = 10, reajsute2 = 7, reajuste3 = 12, reajuste4 = 11;

    printf("Digite seu salario atual: ");
    scanf("%f", &salarioAtual);

    printf("Digite seu cargo atual: ");
    scanf("%f", &cargo);

    if(cargo == 1){
        salarioNovo = ((salarioAtual*reajuste1)/100)+salarioAtual;
        printf("Salario de Vendedor(a) com reajuste: %.2f", salarioNovo);
    }
    if(cargo == 2){
        salarioNovo = ((salarioAtual*reajsute2)/100)+salarioAtual;
        printf("Salario de Auxilar de Escritorio com reajuste: %.2f", salarioNovo);
    }
    if(cargo == 3){
        salarioNovo = ((salarioAtual*reajuste3)/100)+salarioAtual;
        printf("Salario de Auxiliar de Enfermeiro com reajuste: %.2f", salarioNovo);
    }
    if(cargo == 4){
        salarioNovo = ((salarioAtual*reajuste4)/100)+salarioAtual;
        printf("Salario de Bibliotecario(a) com reajuste: %.2f", salarioNovo);
    }
    return 0;
}

