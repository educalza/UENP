/*Fa ̧ca um programa para calcular as ra ́ızes da equa ̧c ̃ao de segundo grau. (ax2 + bx + c = 0).Caso as ra ́ızes n ̃ao existam, uma mensagem deve ser exibida (por exemplo, “N ̃ao existem
ra ́ızes reais”). Caso a equa ̧c ̃ao n ̃ao seja de segundo grau, uma mensagem tamb ́em deve ser
exibida (por exemplo, “N ̃ao  ́e uma equa ̧c ̃ao de segundo grau”). x = (-b +- √(b²-4ac))/2 */

#include <stdio.h>
#include <math.h>

int main(){

    double a, b, c, equa1, equa2, delta;

    printf("Digite o valor de a: ");
    scanf("%lf", &a);

    printf("Digite o valor de b: ");
    scanf("%lf", &b);

    printf("Digite o valor de c: ");
    scanf("%lf", &c);

    delta = sqrt(pow(b,2) -4*a*c);

    if(a == 0){
        printf("Nao eh uma equacao de segundo grau");
    } else if(delta > 0){
        equa1 = (-b + delta) / (2*a);
        equa2 = (-b - delta) / (2*a);
        printf("As raizes sao: %.2lf e %.2lf", equa1, equa2);
    } else if (delta == 0){
        equa1 = -b / (2*a);
        printf("Possui apenas uma raiz real");
    } else
        printf("Nao possui raizes");
/* ou else if (delta < 0){
    printf("nao possui raizes");
}*/
    return 0;
}