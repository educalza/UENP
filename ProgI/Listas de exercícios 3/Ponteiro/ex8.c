/*Utilizando aritm ́etica de ponteiros, mostre como exibir a frase “n ̃ao gosto de programar em
C” como “gosto de programar em C”.*/

#include <stdio.h>
#define N 28

int main(){

    char texto[N] = "nao gosto de programar em C";
    char *p = &texto;

    printf("%s", p+4);
    printf("\n");
    printf("%s", texto+4);

    return 0;
}