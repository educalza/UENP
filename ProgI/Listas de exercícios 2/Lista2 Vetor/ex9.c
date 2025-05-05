/*Fa ̧ca um programa que receba do usu ́ario um vetor de “n” n ́umeros reais e ordene-os em
ordem crescente. Ao final, o programa deve mostrar o vetor ordenado.*/

#include <stdio.h>

int main() {
    int n;

    printf("Digite a quantidade de numeros a serem inseridos: ");
    scanf("%d", &n);
    printf("Digite a sequência de numeros:\n");

    float vetor[n];
    float vetorconta[n];

    for (int i = 0; i < n; i++) {
        printf("Vetor[%d]: ", i);
        scanf("%f", &vetor[i]);
        vetorconta[i] = vetor[i];
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (vetorconta[i] > vetorconta[j]) {
                float temp = vetorconta[i];
                vetorconta[i] = vetorconta[j];
                vetorconta[j] = temp;
            }
        }
    }

    printf("Vetor ordenado em ordem crescente:\n");
    for (int i = 0; i < n; i++) {
        printf("%.2f ", vetorconta[i]);
    }

    return 0;
}
