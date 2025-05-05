/*Fa ̧ca um programa para cadastrar um conjunto de at ́e 100 lojas contendo os seguintes
campos: nome da loja, endere ̧co, telefone e pre ̧co de um determinado produto eletrˆonico.
Em seguida, o programa deve exibir a m ́edia dos pre ̧cos dos produtos cadastrados e uma
rela ̧c ̃ao contendo o nome, endere ̧co e telefone das lojas cujo pre ̧co es ̃ao abaixo da m ́edia
calculada.*/

#include <stdio.h>

struct CadastroLojas{
    char nomeLoja[30];
    char enderecoLoja[50];
    int telefoneLoja;
    float precoProduto;
};
#define L 100

int main(){

    struct CadastroLojas cadastro1[L];

    for(int i=0; i<L; i++){
        printf("----Cadastro loja----\n");
        printf("Nome: ");
        setbuf(stdin, NULL);
        fgets(cadastro1[i].nomeLoja, 30, stdin);
        printf("Edereco: ");
        setbuf(stdin, NULL);
        fgets(cadastro1[i].enderecoLoja, 50, stdin);
        printf("Telefone: ");
        scanf("%d", &cadastro1[i].telefoneLoja);
        printf("Preco do produto: ");
        scanf("%f", &cadastro1[i].precoProduto);
        printf("\n\n");
    }
    printf("\n");
    float soma=0, media=0;

    for(int i=0; i<L; i++){
        soma += cadastro1[i].precoProduto;
        }
        media = soma/L;
        printf("Media de preco dos produtos: %.2f\n\n", media);
        printf("Loja(s) com menor preco:");
        
    for(int i=0; i<L; i++){
        if(cadastro1[i].precoProduto<media){
            printf("Loja: %s", cadastro1[i].nomeLoja);
            printf("Endereco: %s", cadastro1[i].enderecoLoja);
            printf("Telefone: %d\n", cadastro1[i].telefoneLoja);
            printf("Preco: %.2f\n", cadastro1[i].precoProduto);
            printf("\n\n");
        }
    }

    return 0;
}