/*Fa ̧ca uma fun ̧c ̃ao que receba um n ́umero inteiro qualquer, calcule e mostre a tabuada deste
n ́umero.*/

#include <stdio.h>

int main(){
    
    int num, operacao, i;
    int tabuada(int mult, int deno);
    
    printf("Digite um numero inteiro qualquer para receber sua tabuada: ");
    scanf("%d", &num);
    
    
    for(int i=1; i<=10; i++){
        operacao = tabuada(num, i);
        printf("%d x %d = %d \n", i, num, operacao);
    }
}

    int tabuada(int mult, int deno){
    return mult * deno;
}