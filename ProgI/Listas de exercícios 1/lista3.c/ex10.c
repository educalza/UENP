/*Fa ̧ca um programa leia uma sequˆencia de n ́umeros inteiros e mostre quantos n ́umeros
m ́ultiplos de 7 foram digitados no intervalo entre 10 e 50, bem como a m ́edia desses n ́umeros.
O programa deve ser encerrado quando for digitado o valor 0.*/

#include <stdio.h>

int main(){

    int num, count = 0, soma = 0;
    float media;

    printf("Digite numeros(0 para parar): \n");

    for(int i=1; i==i; i++){
        printf("Numero[%d]: ", i);
        scanf("%d", &num);
        if(num>=10 && num<=50){
            if(num%7==0){
                count++;
                soma+=num;
            }
        }    
        if(num==0){
            break;
        }
        if(num<0){
            printf("Por favor, digite numeros maiores que 0");
        }
    }
    media = soma/count;
    printf("%d numeros sao multiplos de 7\n", count);
    printf("A media desses numeros eh: %.2f", media);

    return 0;
}