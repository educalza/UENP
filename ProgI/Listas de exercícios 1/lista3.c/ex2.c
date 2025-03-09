/*Fa ̧ca um programa que receba 2 n ́umeros inteiros quaisquer representados por x e y e
mostre, em ordem crescente, todos os n ́umeros entre x e y cujo resto da divis ̃ao deles por
5 for igual a 2 ou igual a 3.*/

#include <stdio.h>

int main(){

    int x, y;

    printf("Digite um numero para x: ");
    scanf("%d", &x);
    printf("Digite um numerto para : ");
    scanf("%d", &y);

    if(x<y){
        for(int i = x; i<=y; i++){
            if(i%5==2 || i%5==3){
                printf("%d ", i);
            }
        }
    }
    else if(y<x){
        for(int j = y; j<=x; j++){
            if(j%5==2 || j%5==3){
                printf("%d ", j);
            }
        }
    }
    return 0;
}