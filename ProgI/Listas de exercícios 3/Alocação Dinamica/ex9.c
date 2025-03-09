/*Implemente um programa que leia uma string alocada dinamicamente e inverta seu conte ́udo.
Por exemplo, se a string for “alocacao dinamica em c” ent ̃ao dever ́a ser impresso “c me
acimanid oacacola”.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// strlen
int main(){

    char *string = (char*)malloc(sizeof(char));

    printf("Digite a string: ");
    setbuf(stdin, NULL);
    gets(string);

    int tamanho = strlen(string);

    for(int i=tamanho; i>0; i--){
        printf("%c", string[i]);
    }

    return 0;
}