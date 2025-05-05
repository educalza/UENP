/*Implemente uma fun ̧c ̃ao recursiva que retorne verdadeiro quando a string dada  ́e um
pal ́ındromo. Pal ́ındromo  ́e uma palavra que  ́e idˆentica quando a lemos de frente para tr ́as
e tamb ́em de tr ́as para frente. Por exemplo, a palavra “arara”  ́e um pal ́ındromo. J ́a a
palavra “casa” n ̃ao  ́e um pal ́ındromo.*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define N 100

bool StringPalindromo(char *string, int i, int f);

int main(){

    char string[N];

    printf("Digite a string: ");
    setbuf(stdin, NULL);
    fgets(string, N, stdin);

    string[strcspn(string, "\n")] = '\0';

    if(StringPalindromo(string, 0, strlen(string)-1)){
        printf("A string eh um palindramo\n");
    } else{
        printf("A string nao eh um palindramo");
    }

    return 0;
}

bool StringPalindromo(char *string, int i, int f){

    if(i >= f){
        return true;
    }
    if(string[i] != string[f]){
        return false;
    }

    return StringPalindromo(string, i+1, f-1);
}