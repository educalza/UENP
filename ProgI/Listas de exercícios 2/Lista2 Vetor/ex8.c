/*Fa ̧ca um programa receba a idade de v ́arias pessoas e armazene em um vetor, em seguida,
contabilize e mostre o total de pessoas com menos de 18 anos e o total com mais de 30
anos. O programa deve ser encerrado quando for digitado o valor 0.*/

#include <stdio.h>

int main() {
    
    int idade;
    int menorDe18 = 0;
    int maiorDe30 = 0;

    printf("Digite a idade das pessoas (0 para encerrar):\n");

    for (;;) { // o for aqui não possui inicialização, ou seja, o loop roda infinitamente
        printf("Idade: ");
        scanf("%d", &idade);

        if (idade == 0) {
            break;
        }

        if (idade < 18) {
            menorDe18++;
        } else if (idade > 30) {
            maiorDe30++;
        }
    }

    printf("Total de pessoas com menos de 18 anos: %d\n", menorDe18);
    printf("Total de pessoas com mais de 30 anos: %d\n", maiorDe30);

    return 0;
}
