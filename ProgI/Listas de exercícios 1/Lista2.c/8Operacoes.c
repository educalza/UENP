#include <stdio.h>

int main(){

    float x, y, media, diferenca, produto, divisao;
    int codigo;

    printf("Digite o valor de x e y\n");
    printf("X: ");
    scanf("%f", &x);

    printf("Y: ");
    scanf("%f", &y);

    printf("Escolha a operacao a ser realizada:");
    scanf("%d", &codigo);

    switch (codigo){
        case 1: 
            printf("Media aritmetica entre x e y\n");
            media = (x+y)/2;
            printf("Valor da operacao: %.2f", media);
            break;

        case 2:
            printf("Diferenca do maior pelo menor numero\n");
            if(x < y){
                diferenca = y - x;
            }
            else if(x > y){
                diferenca = x - y;
            }
            printf("Valor da operacao: %.2f", diferenca);
            break;

        case 3:
            printf("Produto entre x e y \n");
            produto = x*y;
            printf("Valor da operacao: %.2f", produto);
            break;

        case 4:
            printf("Divisao entre x e y\n");
            divisao = x/y;
            printf("Valor da operacao: %.2f", divisao);
            break;

        default:
            printf("Opcao invalida");
            break;
    }
    return 0;
}