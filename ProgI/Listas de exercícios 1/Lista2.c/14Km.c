/*Fa ̧ca um programa que receba o trajeto em km percorrido por um ve ́ıculo, o tipo do ve ́ıculo
e mostre o consumo estimado de combustvel, sabendo-se que um ve ́ıculo tipo A faz 15 km
com um litro de gasolina, um ve ́ıculo tipo B faz 12 km e um ve ́ıculo tipo C, 10 km por litro.*/

#include <stdio.h>

int main(){

    int tipoVeiculo;
    int km;
    float A, B, C, gasto;

    printf("Digite o tipo de veiculo (A, B ou C): ");
    scanf("%d", &tipoVeiculo);

    printf("Digite o trajeto percorrido pelo veiculo em km: ");
    scanf("%d", &km);

    A = 15/km; //km/litro
    B = 12/km;
    C = 10/km;

    if(tipoVeiculo == 1){
        gasto = 15/km;
        printf("Consumo de combustivel: %d", gasto);
    }else if(tipoVeiculo == B){
        printf("Consumo estimado de combustivel: %d", B);
    }else if(tipoVeiculo == C){
        printf("Consumo estimado de combustivel: %d", C);
    }
    return 0; 
}