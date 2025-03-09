/*Fa ̧ca um programa para criptografar uma string qualquer informada pelo usu ́ario, substituindo
as vogais encontradas na string pelo caractere especial asterisco (*).*/

#include <stdio.h>
#include <string.h>
#define N 100

void criptografar(char string[]) {
    int tamanho = strlen(string);

    for (int i = 0; i < tamanho; i++) {
        char c = string[i];

        if (c == 'a' || c == 'A' || c == 'e' || c == 'E' || c == 'i' || c == 'I' || c == 'o' || c == 'O' || c == 'u' || c == 'U') {
            string[i] = '*';
        }
    }
}

int main() {
    char string[N];

    printf("Digite uma string (max 100):\n");
    fgets(string, N, stdin);

    criptografar(string);

    printf("String criptografada: %s\n", string);

    return 0;
}
