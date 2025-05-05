/*Fa ̧ca um programa que leia uma string e, em seguida, implemente uma fun ̧c ̃ao que contabiliza
a quantidade de vogais contidas na string. Lembre-se que as vogais podem estar em
mai ́usculas e/ou min ́usculas.*/
#include <stdio.h>
#include <string.h>
#define N 50

int contarVogais(char string[]) {
    int tamanho = strlen(string);
    int count = 0;

    for (int i = 0; i < tamanho; i++) {
        char c = string[i];
        if ((c == 'a' || c == 'A') ||
            (c == 'e' || c == 'E') ||
            (c == 'i' || c == 'I') ||
            (c == 'o' || c == 'O') ||
            (c == 'u' || c == 'U')) {
            count++;
        }
    }

    return count;
}

int main() {
    char string[N];

    printf("Digite: (max 50)\n");
    fgets(string, N, stdin);

    int vogais = contarVogais(string);

    printf("Quantidade de vogais na string: %d\n", vogais);

    return 0;
}
