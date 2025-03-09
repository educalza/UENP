#include <stdio.h>

int main() {
    int interfaceGrafica, inteligenciaArtificial, encapsulamento, indentacao, structs;

    printf("Digite os requisitos atendidos (0 - não atendido, 1 - atendido):\n");
    printf("Interface Gráfica: ");
    scanf("%d", &interfaceGrafica);
    printf("Inteligência Artificial: ");
    scanf("%d", &inteligenciaArtificial);
    printf("Encapsulamento: ");
    scanf("%d", &encapsulamento);
    printf("Indentação: ");
    scanf("%d", &indentacao);
    printf("Structs: ");
    scanf("%d", &structs);

    if (interfaceGrafica == 1 || inteligenciaArtificial == 1) {
        if (encapsulamento == 1 && indentacao == 1 && structs == 1) {
            printf("Trabalho será avaliado.\n");
        } else {
            printf("Trabalho não atendeu aos requisitos. Nota 0.\n");
        }
    } else {
        printf("Trabalho não atendeu aos requisitos. Nota 0.\n");
    }

    return 0;
}