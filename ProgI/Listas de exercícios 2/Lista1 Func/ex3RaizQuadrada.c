/*Fa ̧ca um programa para calcular as ra ́ızes da equa ̧c ̃ao de 2o grau. O programa recebe como
entrada os n ́umeros a, b, c, que correspondem aos coeficientes da equa ̧c ̃ao de segundo grau
(ax2 + bx + c = 0). Em seguida, o programa deve mostrar as ra ́ızes da equa ̧c ̃ao. Caso
as ra ́ızes n ̃ao existam, uma mensagem deve ser exibida (por exemplo, “N ̃ao existem ra ́ızes
reais”). Caso a equa ̧c ̃ao n ̃ao seja de segundo grau, uma mensagem tamb ́em deve ser exibida
(por exemplo, “N ̃ao  ́e uma equa ̧c ̃ao de segundo grau”).
Para isso, implemente fun ̧c ̃oes para calcular e retornar o valor de delta e para calcular e
retornar os valores das raizes x1 e x2 da equa ̧c ̃ao.*/

#include <stdio.h>
#include <math.h>

int main(){

    float a, b, c, operacao, raiz(float a, float b, float c, float equa1, float equa2), equa1, equa2;

    printf("Digite o valor de a, b e c da equacao de segundo grau: \n");
    printf("a: ");
    scanf("%f", &a);
    printf("b: ");
    scanf("%f", &b);
    printf("c: ");
    scanf("%f", &c);

    operacao = raiz(a, b, c, equa1, equa2);
    printf("Raizes da equacao de segundo grau: \n x1: %.2f \n x2: %.2f", operacao);
}

float raiz(float a, float b, float c, float equa1, float equa2){
    float delta;
    delta = sqrt(pow(b, 2)-4*a*c);
    equa1 = (-b+delta)/(2*a);
    equa2 = (-b-delta)/(2*a);
    if(delta<=0)
        printf("Nao existe raizes reais");
    return equa1 && equa2;
}

