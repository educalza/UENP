// Ler um valor de raio de um círculo e calcular o perímetro e área.
//    p = 2 * pi * raio     a = pi * raio²
#include <stdio.h>
#define pi 3.14159

int main(){

    float raio, perimetro, area;

    printf("Digite um valor para o raio: ");
    scanf("%f", &raio);

    perimetro = 2 * pi * raio;
    area = pi * raio * raio;

    printf("Valor do perimetro:%.3f\n", perimetro);
    printf("Valor da area:%.3f\n", area);

return 0;

}
