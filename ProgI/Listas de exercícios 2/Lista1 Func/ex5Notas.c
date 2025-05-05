/*Implemente uma fun ̧c ̃ao que receba como parˆametro 2 notas e uma op ̧c ̃ao correspondendo
ao c ́alculo da m ́edia. Caso a op ̧c ̃ao seja a letra “a”, deve ser calculada a m ́edia aritm ́etica;
caso a op ̧c ̃ao seja a letra “p”, deve ser calculada a m ́edia ponderada (pesos 3 e 2); caso a
op ̧c ̃ao seja a letra “h”, deve ser calculada a m ́edia harmˆonica. Retorne a m ́edia devidamente
calculada para o programa principal.*/

#include <stdio.h>

double MediaAritmetica(double nota1, double nota2) {
    return (nota1 + nota2) / 2.0;
}

double MediaPonderada(double nota1, double nota2) {
    return (3 * nota1 + 2 * nota2) / 5.0;
}

double MediaHarmonica(double nota1, double nota2) {
    return 2.0 / ((1.0 / nota1) + (1.0 / nota2));
}

int main() {
    double nota1, nota2;
    char opcao;
    double resultado;

    printf("Digite a primeira nota: ");
    scanf("%lf", &nota1);
    printf("Digite a segunda nota: ");
    scanf("%lf", &nota2);
    printf("Digite a opcao ('a' para media aritmetica, 'p' para media ponderada, 'h' para media harmonica): ");
    scanf(" %c", &opcao);

    switch (opcao) {
        case 'a':
            resultado = MediaAritmetica(nota1, nota2);
            break;
        case 'p':
            resultado = MediaPonderada(nota1, nota2);
            break;
        case 'h':
            resultado = MediaHarmonica(nota1, nota2);
            break;
        default:
            printf("Opcao invalida!\n");
            return 1;
    }

    printf("A media calculada e: %.2lf\n", resultado);

    return 0;
}
