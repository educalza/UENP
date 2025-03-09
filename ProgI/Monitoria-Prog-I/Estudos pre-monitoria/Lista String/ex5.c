/*Fa¸ca um programa que leia uma string e, em seguida, implemente uma fun¸c˜ao que contabiliza
a quantidade de vogais contidas na string. Lembre-se que as vogais podem estar em
mai´usculas e/ou min´usculas.*/

#include <stdio.h>
#include <string.h>
#define MAX 255

void contarVogais(char str[]){

    char vogalMinuscula[5] = {'a', 'e', 'i', 'o', 'u'};
    char vogalMaiuscula[5] = {'A', 'E', 'I', 'O', 'U'};
    int count=0;
    for(int i=0; str[i]!='\0'; i++){
        for(int j=0; j<5; j++){
            if(str[i] == vogalMinuscula[j] || str[i] == vogalMaiuscula[j]){
                count++;
            }
        }
    }
    printf("A string tem o total de %d vogais.\n", count);
}

int main(){

    char str[MAX];
    printf("Digite: ");
    fgets(str, MAX, stdin);

    contarVogais(str);
}