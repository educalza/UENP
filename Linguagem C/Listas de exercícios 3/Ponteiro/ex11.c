/*Fa ̧ca um programa que leia uma string de no m ́aximo 100 caracteres. Em seguida, implemente
uma fun ̧c ̃ao para calcular e mostrar o total de palavras da string lida. Para isso, utilize o
prot ́otipo de fun ̧c ̃ao a seguir.*/

#include <stdio.h>
#include <string.h>
#define N 100

int main(){

    char string[N];

    printf("Escreva(max 100): ");
    fgets(string, 100, stdin);

    char *strin = string;
    int funcao, totalPalavras(char *str);
    funcao = totalPalavras(strin);

    printf("Total de palavras: %d", funcao);

}



int totalPalavras(char *str){
    
    int count=1;

    for(int i=0; i<N; i++){
        if(*(str+i)==' '){
            count++;   
        }
    }
    return count;
}