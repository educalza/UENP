/*Fa ̧ca uma que receba uma determinada hora expressa no formato de horas, minutos e
segundos e, em sequida, transforme no valor correspondente em segundos.*/

#include  <stdio.h>

int main(){

    int h, min, seg, m;
    int segundosTotal(int horas, int minutos, int segundos);

    printf("Digite as horas: ");
    scanf("%d", &h);
    printf("Digite os minutos: ");
    scanf("%d", &min);
    printf("Digite os segundos: ");
    scanf("%d", &seg);

    m = segundosTotal(h, min, seg);

    printf("Segundos totais do tempo digitado: %d", m);
}

int segundosTotal(int horas, int minutos, int segundos){

    return (horas*3600)+(minutos*60)+segundos;
}