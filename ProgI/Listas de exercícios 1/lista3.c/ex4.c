/*Fa ̧ca um programa que calcule e exiba o valor de S, em que S  ́e dado por: */

#include <stdio.h>

int main(){

    int x = 99, count = 1;
    float s = 0.0;

    printf("Valor de x final 99, valor de y final 50\n");

    for(int i = 1; i<=x; i++){
        if(x%2!=0){
            s = s + (x/count++);
        }
    }
    printf("soma: %.2f", s);

    return 0;
}