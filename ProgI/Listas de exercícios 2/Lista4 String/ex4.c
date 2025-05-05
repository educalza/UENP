/*Fa ̧ca um programa que leia duas strings quaisquer e mostre se as duas s ̃ao iguais ou
diferentes no conte ́udo e se possuem o mesmo tamanho. Lembre-se que as strings podem
conter letras mai ́usculas e/ou min ́usculas.*/
#include <stdio.h>
#include <string.h>
#define N 50

int main(){

    char string[N];
    char string2[N];
    int igual=0;

    printf("Digite: \n");
    fgets(string, 50, stdin);
    fflush(stdin);
    printf("Digite novamente: \n");
    fgets(string2, 50, stdin);
    fflush(stdin);

    igual = strcmp(string, string2); //segunda igual a primeira? comparação

    if(igual == 0)
        printf("Os textos sao iguais.");
    else
        printf("Os textos nao sao iguais.");
        
    return 0;
}
