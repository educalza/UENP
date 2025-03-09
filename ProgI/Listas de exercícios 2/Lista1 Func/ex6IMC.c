/*Implemente uma fun ̧c ̃ao que receba como parˆametro o peso (em quilos) e a altura (metros)
de uma pessoa, calcule e retorne o seu IMC ( ́Indice de Massa Corporal), cujo c ́alculo  ́e dado
por: IMC = peso/(altura * altura). Implemente outra fun ̧c ̃ao que receba como parˆametro
o IMC de uma pessoa e a classifique de acordo com a Tabela 1.*/

#include <stdio.h>

int main(){

    float peso, altura, IMC;
    float imc(float pesoKG, float alturaM);

    printf("Digite seu peso em quilos: ");
    scanf("%f", &peso);

    printf("Digite sua altura em metros: ");
    scanf("%f", &altura);

    IMC = imc(peso, altura);
    printf("Seu indice de Massa Corporal(IMC) eh de %.2f\n\n", IMC);

    classificacao();
    return 0;
}

float imc(float pesoKG, float alturaM){
    return pesoKG/(alturaM*alturaM);
}

void classificacao(){

    float imc;

    printf("Digite seu IMC para classificar sua condicao: ");
    scanf("%f", &imc);

    if(imc<18.5){
        printf("Abaixo do peso!");
    }if(imc>=18.5 && imc<=24.9){
        printf("Peso normal.");
    }if(imc>=25 && imc<=29.9){
        printf("Sobrepeso!");
    }if(imc>=30){
        printf("Obeso!");
    }
}