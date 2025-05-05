// ler a hora, minutos e segundos e, trasnformar tudo em segundos
#include <stdio.h>

int main() {

    int hora, minutos, segundos;

    printf("Digite a hora:");
    scanf("%d", &hora);

    printf("Digite os minutos:");
    scanf("%d", &minutos);

    printf("Digite os segundos:");
    scanf("%d", &segundos);

    int segundosFinal;

    segundosFinal = hora * 3600 + minutos * 60 + segundos;

    printf("Total de segundos no horario informado e: %d segundos.\n", segundosFinal);

    return 0;

}