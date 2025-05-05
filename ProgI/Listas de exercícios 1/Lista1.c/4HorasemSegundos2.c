// ler a hora, minutos e segundos e, trasnformar tudo em segundos
#include <stdio.h>

int main() {

    int hora, minutos, segundos;

    printf("Digite a hora, os minutos e os segundos:");
    scanf("%d%d%d", &hora, &minutos, &segundos);

    int segundosFinal;

    segundosFinal = hora * 3600 + minutos * 60 + segundos;

    printf("Total de segundos do horario informado e: %d segundos.\n", segundosFinal);

    return 0;

}