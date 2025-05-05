#include <stdio.h>

struct Processo {
    int id;
    int burst_time;
    int waiting_time;
    int turnaround_time;
};

void ordenarPorBurstTime(struct Processo p[], int n) {
    struct Processo temp;
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (p[j].burst_time < p[i].burst_time) {
                temp = p[i];
                p[i] = p[j];
                p[j] = temp;
            }
        }
    }
}

void calcularTempos(struct Processo p[], int n) {
    p[0].waiting_time = 0;
    p[0].turnaround_time = p[0].burst_time;

    for (int i = 1; i < n; i++) {
        p[i].waiting_time = p[i-1].waiting_time + p[i-1].burst_time;
        p[i].turnaround_time = p[i].waiting_time + p[i].burst_time;
    }
}

void mostrarResultados(struct Processo p[], int n) {
    int total_waiting = 0, total_turnaround = 0;
    
    printf("ID\tBurst\tEspera\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("P%d\t%d\t%d\t%d\n", p[i].id, p[i].burst_time, p[i].waiting_time, p[i].turnaround_time);
        total_waiting += p[i].waiting_time;
        total_turnaround += p[i].turnaround_time;
    }

    printf("\nTempo médio de espera: %.2f\n", (float)total_waiting / n);
    printf("Tempo médio de turnaround: %.2f\n", (float)total_turnaround / n);
}

int main() {
    int n;
    printf("Quantos processos? ");
    scanf("%d", &n);

    struct Processo processos[n];

    for (int i = 0; i < n; i++) {
        processos[i].id = i + 1;
        printf("Tempo de burst do processo P%d: ", i + 1);
        scanf("%d", &processos[i].burst_time);
    }

    ordenarPorBurstTime(processos, n);
    calcularTempos(processos, n);
    mostrarResultados(processos, n);

    return 0;
}
