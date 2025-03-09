/*Fa ̧ca um programa que receba como entrada trˆes n ́umeros positivos em ponto flutuante,
em seguida, mostre quantos deles est ̃ao acima da m ́edia aritm ́etica.*/

#include <stdio.h>

int main(){

    float x, y, z, mediaAritmetica;
    int NumeroMenor = 0;
    
    printf("Digite o valor racional para x, y e z: \n");
    printf("x: ");
    scanf("%f", &x);

    printf("y: ");
    scanf("%f", &y);

    printf("z: ");
    scanf("%f", &z);

    mediaAritmetica = (x+y+z) / 3;

    if (x>mediaAritmetica){
        printf("x eh maior que a media aritimetica\n");
        printf("x = %.2f, media = %.2f\n", x, mediaAritmetica);
        NumeroMenor = 1;
    } 
    if (y>mediaAritmetica){
        printf("y eh maior que a media aritimetica\n");
        printf("x = %.2f, media = %.2f\n", x, mediaAritmetica);
        NumeroMenor = 1;
    } 
    if (z>mediaAritmetica){
        printf("z eh maior que a media aritimetica\n");
        printf("x = %.2f, media = %.2f\n", x, mediaAritmetica);
        NumeroMenor = 1;
    }

    if (NumeroMenor == 0){
        printf("Nenhum numero eh maior que a media");
    }
    return 0;
}