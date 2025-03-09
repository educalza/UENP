/*Fa ̧ca um programa que receba dois n ́umeros inteiros representando o intervalo (inicio) e
(fim) de uma sequˆencia, e depois imprima todos os n ́umeros  ́ımpares dentro deste intervalo.*/

#include <stdio.h>

int main(){
    
    int num1, num2;

    printf("Digite o numero inicial do intervalo: ");
    scanf("%d", &num1);
    printf("Digite o numero final do intervalo: ");
    scanf("%d", &num2);

    for(int i = num1; i<=num2; i++){
        if(i%2!=0)
        printf("%d ", i);
    }
    return 0;
}