/*Fa ̧ca um programa para gerenciar a ficha de registro de passageiros de ˆonibus de uma
empresa. Os ˆonibus possuem 44 lugares numerados de 1 a 44. O programa deve ler
uma lista de registro de passageiros, com o n ́umero da passagem, data, de (origem), para
(destino), hor ́ario, poltrona, idade e nome do passageiro. A entrada termina quando o valor
do n ́umero da passagem for −1.
Ao final, o programa deve exibir o nome de todos os passageiros que estiverem acima da
m ́edia das idades e que estiverem sentados nas poltronas pares.*/

#include <stdio.h>
#define P 44

struct Passageiro {
    int numeroPassagem;
    int dataOrigemDia;
    int dataOrigemMes;
    int dataOrigemAno;
    int dataDestinoDia;
    int dataDestinoMes;
    int dataDestinoAno;
    int horario;
    int poltrona;
    int idade;
    char nome[50];
};

float calcularMediaIdades(struct Passageiro passageiros[], int quantidade) {

    int somaIdades = 0;

    for (int i = 0; i < quantidade; i++) {
        somaIdades += passageiros[i].idade;
    }

    return quantidade > 0 ? (float)somaIdades / quantidade : 0;
}

int main() {

    struct Passageiro passageiros[P];

    int contador = 0;

    printf("Digite as informacoes dos passageiros (ou -1 para encerrar):\n");

    for (int i = 0; i < P; i++) {
        printf("Numero da passagem: ");
        scanf("%d", &passageiros[i].numeroPassagem);

        if (passageiros[i].numeroPassagem == -1) {
            break;
        }

        printf("Data de origem (Dia Mes Ano): ");
        scanf("%d %d %d", &passageiros[i].dataOrigemDia, &passageiros[i].dataOrigemMes, &passageiros[i].dataOrigemAno);

        printf("Data de destino (Dia Mes Ano): ");
        scanf("%d %d %d", &passageiros[i].dataDestinoDia, &passageiros[i].dataDestinoMes, &passageiros[i].dataDestinoAno);

        printf("Horario: ");
        scanf("%d", &passageiros[i].horario);

        printf("Poltrona: ");
        scanf("%d", &passageiros[i].poltrona);

        printf("Idade: ");
        scanf("%d", &passageiros[i].idade);

        printf("Nome: ");
        scanf("%s", passageiros[i].nome);

        contador++;
    }

    float mediaIdades = calcularMediaIdades(passageiros, contador);

    printf("\nPassageiros acima da media de idade e em poltronas pares:\n");

    for (int i = 0; i < contador; i++) {
        if (passageiros[i].idade > mediaIdades && passageiros[i].poltrona % 2 == 0) {
            printf("%s\n", passageiros[i].nome);
        }
    }

    return 0;
}
