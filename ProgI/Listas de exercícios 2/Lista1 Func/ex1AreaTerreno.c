/*Fa ̧ca uma fun ̧c ̃ao para calcular a  ́area de “n” terrenos retangulares, em que “n”  ́e informado
pelo usu ́ario. O programa recebe como entrada o comprimento e a largura de cada terreno
e, ao final, exibe a sua  ́area correspondente.*/

// função para calcular a área dos terrenos retangulares.

#include <stdio.h>

int main(){

    int terrenos;
    float comprimento, largura, area(float c, float l);
    float a = 0, c = 0, l = 0;

    printf("Digite quantos terrenos sera analizados: ");
    scanf("%d", &terrenos);
    printf("Digite o comprimento e a largura de cada terreno\n\n");

    for(int i = 1; i <= terrenos; i++){
        printf("Terreno %d\n", i);
        printf("Comprimento: ");
        scanf("%f", &comprimento);
        printf("Largura: ");
        scanf("%f", &largura);
        
        a = area(comprimento, largura); 
        printf("Area do terreno retangualar: %.2f\n\n", a);
    }
}

float area(float c, float l){
    return c * l;
}