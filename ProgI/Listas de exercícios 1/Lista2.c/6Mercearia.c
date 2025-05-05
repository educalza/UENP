/*Fa ̧ca uma programa que leia o c ́odigo do produto, a quantidade comprada e exiba o valor
que o cliente deve pagar, j ́a considerando o desconto quando aplic ́avel.    desconto quando
compra de 15 produtos, ou preço maior que 40 reais*/

#include <stdio.h>

int main(){

    int QuantidaDeCompra, desconto = 15, produto;
    float valorTotal, valorDescontado;
    
    printf("Digite codigo do produto: ");
    scanf("%d", &produto);

    if (produto == 1){
        printf("Item 1 selecionado\n");
        printf("Digite a quantidade de produto a ser comprada: ");
        scanf("%d", &QuantidaDeCompra);
        valorTotal = QuantidaDeCompra*5.30;
        if(valorTotal >= 40 || QuantidaDeCompra >= 15){
            valorDescontado = (((valorTotal*desconto)/100) - valorTotal)*-1;
            printf("Valor a ser pago com desconto aplicado: %.2f", valorDescontado);
        }else 
            printf("Valor sem desconto: %.2f", valorTotal);
    }
            if (produto == 2){
            printf("Item 2 selecionado\n");
            printf("Digite a quantidade de produto a ser comprada: ");
            scanf("%d", &QuantidaDeCompra);
            valorTotal = QuantidaDeCompra*6;
                if(valorTotal >=40 || QuantidaDeCompra >=15){
                    valorDescontado = (((valorTotal*desconto)/100) - valorTotal)*-1;
                    printf("Valor a ser pago com desconto aplicado: %.2f", valorDescontado);
                }else
                    printf("Valor sem desconto: %.2f", valorTotal);
            }         
                    if(produto == 3){
                        printf("Item 3 selecionado\n");
                        printf("Digite a quantidade de produto a ser comprada: ");
                        scanf("%d", &QuantidaDeCompra);
                        valorTotal = QuantidaDeCompra*3.20;
                        if(valorTotal >= 40 || QuantidaDeCompra >= 15){
                            valorDescontado = (((valorTotal*desconto)/100) - valorTotal)*-1;
                            printf("Valor a ser pago com desconto aplicado: %.2f", valorDescontado);
                        }else
                            printf("Valor sem desconto: %.2f", valorTotal);
                    }
                            if(produto == 4){
                                printf("Item 4 selecionado\n");
                                printf("Digite a quantidade de produto a ser comprada: ");
                                scanf("%d", &QuantidaDeCompra);
                                valorTotal = QuantidaDeCompra*2.50;
                                if(valorTotal >= 40 || QuantidaDeCompra >= 15){
                                    valorDescontado = (((valorTotal*desconto)/100) - valorTotal)*-1;
                                    printf("Valor a ser pago com desconto aplicado: %.2f", valorDescontado);
                                }else
                                    printf("Valor sem desconto");
                            }
    return 0;
}