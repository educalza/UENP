/*Fa ̧ca um programa que determine e mostre os 50 primeiros n ́umeros m ́ultiplos de 3 considerando
n ́umeros maiores que 0.*/

#include <stdio.h>

int main(){

    printf("Primeiros 50 numeros multiplos de 3: \n");

    for(int i = 1; i<=50; i++){
        if(i%3==0){
            printf("%d ", i);
        }
    }
    return 0;
}