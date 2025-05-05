/*Fa¸ca um programa que leia uma string e um caractere qualquer e, em seguida, implemente
uma fun¸c˜ao que contabilize e retorne o total de ocorrˆencias desse caractere na string*/

#include <stdio.h>
#define N 255

int quantiaDeCaracteres(char str[], char caracter){

    int count=0;
    for(int i=0; str[i]!='\0'; i++){
        if(str[i] == caracter){
            count++;
        }
    }
    return count;
}

int main(){

    char str[N];

    printf("Digite uma frase: ");
    fflush(stdin);
    fgets(str, N, stdin);

    char caracter;
    printf("\nDigite um caracter: ");
    scanf("%c", &caracter);

    printf("Quantia do caracter na string: %d\n", quantiaDeCaracteres(str, caracter));

    return 0;
}