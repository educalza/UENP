/*Fa ̧ca um programa que leia uma string informada pelo usu ́ario e, em seguida, implemente
uma fun ̧c ̃ao que contabilize a quantidade de espa ̧cos em branco existente na string lida.*/

#include <stdio.h>
#include <string.h>
#define N 200

int contarEspacos(char string[]) {
    int count = 0;
    int tamanho = strlen(string);

    for (int i = 0; i < tamanho; i++) {
        if (string[i] == ' ') {
            count++;
        }
    }

    return count;
}

int main() {
    char string[N];
    
    printf("Digite: (max 200)\n");
    fgets(string, N, stdin);

    int espacos = contarEspacos(string);

    printf("Espacos em branco apareceram %d vezes.\n", espacos);
    return 0;
}
