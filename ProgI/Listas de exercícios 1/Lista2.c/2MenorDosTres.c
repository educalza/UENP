/*Fa ̧ca um programa que receba como entrada trˆes n ́umeros inteiros e, em seguida, exiba o
menor deles.*/

#include <stdio.h>

int main(){

    int x, y, z;

    printf("Digite o valor inteiro para x, y e z: \n");
    printf("x: ");
    scanf("%d", &x);
    
    printf("y: ");
    scanf("%d", &y);

    printf("z: ");
    scanf("%d", &z);

    if (x<y && x<z){
        printf("O menor numero eh o x: %d", x);
    } else if (y<x && y<z){
        printf("O menor numero eh o y: %d", y);
    } else if (z<x && z<y){
        printf("O menor numero eh o z: %d", z);
    }
    return 0;
}