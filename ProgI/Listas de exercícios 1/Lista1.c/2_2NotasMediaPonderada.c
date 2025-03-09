// ler 4 notas, atribuir pesos (1.5, 2, 3, 3.5) e tirar a média ponderada
#include <stdio.h>

int main(){

    float nota1, nota2, nota3, nota4;
    
    printf("Digite suas notas:");
    scanf("%f%f%f%f", &nota1, &nota2, &nota3, &nota4);

    float pesos, mediaPonderada;
    pesos = nota1 * 1.5, nota2 * 2, nota3 * 3, nota4 * 3.5;
    mediaPonderada = pesos / (1.5 + 2 + 3 + 3.5);

    printf("Media ponderada de suas notas:%.2f\n", mediaPonderada);

    return 0;

}