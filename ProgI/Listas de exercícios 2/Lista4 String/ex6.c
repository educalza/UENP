/*Fa ̧ca um programa que leia uma string e um caractere qualquer e, em seguida, implemente
uma fun ̧c ̃ao que contabilize e retorne o total de ocorrˆencias desse caractere na string.*/

#include <stdio.h>
#include <string.h>
#define N 100

int contarOcorrencias(char string[], char caracter) {
    int tamanho = strlen(string);
    int count = 0;

    for (int i = 0; i < tamanho; i++) {
        if (string[i] == caracter) {
            count++;
        }
    }

    return count;
}

int main() {
    char string[N], caracter;
    
    printf("Digite uma string (max 100):\n");
    fgets(string, N, stdin);

    printf("Digite um caracter:\n");
    scanf(" %c", &caracter); 

    int ocorrencias = contarOcorrencias(string, caracter);

    printf("O caractere '%c' apareceu %d vezes na string.\n", caracter, ocorrencias);

    return 0;
}
