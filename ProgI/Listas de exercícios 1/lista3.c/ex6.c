/*Fa ̧ca um programa receba a idade de v ́arias pessoas, contabilize e mostre o total de pessoas
com menos de 18 anos e o total com mais de 30 anos. O programa deve ser encerrado
quando for digitado o valor 0.*/

#include <stdio.h>

int main(){

    int idade, Menor18 = 0, Maior30 = 0;

    printf("Digite a idade das pessoas(digitar 0 para encerrar): \n");

    for(int i=0; i==i; i++){
        printf("Pessoa[%d]:", i+1);
        scanf("%d", &idade);
        if(idade<18 && idade!=0){
            Menor18++;
        }
        if(idade>30){
            Maior30++;
        }
        if(idade==0){
            break;
        }
    }
    printf("Ha %d, com idade inferior que 18 anos.\n", Menor18);
    printf("Ha %d, com idade maior que 30 anos.\n", Maior30);

    return 0;
}