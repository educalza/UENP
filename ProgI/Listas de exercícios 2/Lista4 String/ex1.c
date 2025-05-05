/*Fa ̧ca um programa que inverta uma string lida. Por exemplo, se a string digitada for
programar em linguagem C, o resultado deve ser C megaugnil me ramargorp.*/
#include <stdio.h>
#include <string.h>
#define S 50

int main(){

    char string[S];
    int tamanho;

    printf("Digite: \n");
    gets(string);

    tamanho = strlen(string);

    for(int i=tamanho; i>=0; i--){
        printf("%c", string[i]);
    }
    return 0;
}
