/*Fa ̧ca uma fun ̧c ̃ao que receba como parˆametro um n ́umero inteiro e verifique se este  ́e primo
ou n ̃ao.*/

#include <stdio.h>

int main(){

    printf("Digite um numero para conferir se ele eh primo ou nao: \n");

    primo();
    return 0;
}

void primo(void){
    
    int num, count = 0;

    scanf("%d", &num);

    for(int i = 1; i<=num; i++){
        if(num%i==0){
            count++;
        }
    }
    if(count == 2){
        printf("Esse numero eh primo");
    }else{
        printf("Essse numero nao eh primo");
    }
}