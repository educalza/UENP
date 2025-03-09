/*Fa ̧ca um programa que receba a altura e a idade de uma pessoa, identifique quantos
brinquedos ele pode andar.*/
#include <stdio.h>

int main(){

    int idade, contador = 0;
    float altura;

    printf("Digite sua idade: ");
    scanf("%d", &idade);

    printf("Digite sua altura em centimetros: ");
    scanf("%f", &altura);

    if(altura >= 150 && idade >= 12){
        printf("Autorizado em andar na Barca Viking\n");
        contador++;
    }else 
        printf("Nao possui autorizacao para andar na Barca Viking\n");

    if(altura >= 140 && idade >= 14){
        printf("Autorizado em andar no Elevador of Death\n");
        contador++;
    }else
        printf("Nao possui autorizacao para andar no Elevador of Death\n");

    if(altura >= 170 && idade >= 16){
        printf("Autorizado em andar no Final Killer\n");
        contador++;
    }else
        printf("Nao possui autorizacao para andar no Final Killer\n");

    printf("Quantidade de brinquedos que voce pode andar: %d\n", contador);
    return 0;
}