#include <stdio.h>

int main(){

    int desconto = 15, quantidade, produto;
    float valorProduto, valorTotal, valorDescontado;

    printf("Digite o codigo do produto(1, 2, 3 ou 4): ");
    scanf("%d", &produto);

    printf("Digite a quantidade a ser comprada: ");
    scanf("%d", &quantidade);

    printf("Digite o valor do produto(1 = 5.30, 2 = 6, 3 = 3.20 e 4 = 2.50): ");
    scanf("%f", &valorProduto);

    valorTotal = quantidade*valorProduto;

    if(valorTotal >= 40 || quantidade >= 15){
        valorDescontado = (((valorTotal*desconto)/100)-valorTotal)*-1;
        printf("Valor total com desconto aplicado: %.2f", valorDescontado);
    } else
        printf("Valor sem desconto aplicado: %.2f", valorTotal);
    return 0;
}