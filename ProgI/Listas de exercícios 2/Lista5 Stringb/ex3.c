/*Fa ̧ca um progrma que leia uma string de tamanho qualquer, em seguida, leia um caractere
e mostre as posi ̧c ̃oes ( ́ındices) de onde ocorre o caractere na string. Caso n ̃ao haja nenhuma
ocorrˆencia do caractere na string, deve ser impresso o valor -1.*/


#include <stdio.h>
#include <ctype.h>
#define MAX 100

void Caractere(char *String, char caractere){

    int i, j = -1;

    for(i = 0; String[i] != '\0'; i++){
        if(tolower(String[i]) == caractere){
            printf("\n'%c'Aparece na posicao %d", caractere, i + 1);
            j++;
            }
    }
    printf("\n");
    if (j + 1> 0){
        printf("\nExistem %d '%c' na string inteira.",j + 1, caractere);
    }

    else{
        printf("\n%d", j);
    }
}
int main(){

    char String[MAX + 1], caractere;

    printf("Escreva uma frase:\n");
    fgets(String, MAX+1, stdin);

    printf("Escreva um caractere:\n");
    scanf("%c", &caractere);

    Caractere(String, caractere);

return 0;
}