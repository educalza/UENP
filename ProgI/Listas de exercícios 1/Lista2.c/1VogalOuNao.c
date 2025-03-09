/*Fa ̧ca um programa que receba como entrada um caractere e informe se ele  ́e uma vogal ou
n ̃ao.*/

#include <stdio.h>

int main(){

    char caractere;

    printf("Digite um caractere: ");
    scanf("%c", &caractere);

    if (caractere == 'a' || caractere == 'A'||
        caractere == 'e' || caractere == 'E'||
        caractere == 'i' || caractere == 'I'||
        caractere == 'o' || caractere == 'O'||
        caractere == 'u' || caractere == 'U'){
        printf("O caracter '%c' eh uma vogal\n", caractere);
    } else {
        printf("O caracter '%c' nao eh uma vogal.", caractere);
    }
return 0;
}