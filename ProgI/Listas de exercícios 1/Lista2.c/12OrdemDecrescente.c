/*Fa ̧ca um programa que receba como entrada 3 n ́umeros inteiros e e exiba esses n ́umeros em
ordem decrescente. Desafio: tente utilizar apenas 4 comandos if.*/

#include <stdio.h>

int main(){

    int x, y, z;

    printf("Digite o valor para x, y e z");
    printf("Valor de x: ");
    scanf("%d", &x);

    printf("Valor de y: ");
    scanf("%d", &y);

    printf("Valor de z: ");
    scanf("%d", &z);

    if(x < y && x < z && y < z){
        printf("Ordem descresente: %d%d%d", x, y, z);
    }if(x < z && x < y && y < z)
}