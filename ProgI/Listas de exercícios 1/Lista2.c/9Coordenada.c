#include <stdio.h>

int main(){

    float x, y, coordenadax, coordenaday;

    printf("Digite o valor de x e y\n");
    
    printf("Valor de x: ");
    scanf("%f", &x);

    printf("Valor de y: ");
    scanf("%f", &y);

    if(x > 0 && y > 0){
        printf("Primeiro quadrante");
    }else if(x < 0  && y > 0){
        printf("Segundo quadrante");
    }else if(x > 0 && y < 0){
        printf("Terceiro quadrante");
    }else if(x < 0 && y < 0){
        printf("Quarto quadrante");
    }
    return 0;
}