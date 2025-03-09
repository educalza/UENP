/*Fa ̧ca um programa para cadastrar o card ́apio de um restaurante. O programa deve ler um
inteiro n, representando o n ́umero de produtos a serem cadastrados, seguido pelo cadastro
dos n produtos, em que ser ̃ao lidos para cada produto, o c ́odigo de identifica ̧c ̃ao, a descri ̧c ̃ao
do produto e o pre ̧co. Em seguida, s ̃ao lidos os pedidos. O pedido consiste do c ́odigo do
produto e da quantidade de itens daquele produto que ser ́a pedido. O pedido se encerra
quando o c ́odigo lido  ́e igual a 0. Se o cliente pede um produto n ̃ao cadastrado ou uma
quantidade negativa o pedido  ́e considerado inv ́alido.
Ao final, o programa deve calcular quantos itens o cliente escolheu de cada c ́odigo e exibir
o valor que o cliente deve pagar. Pedidos inv ́alidos s ̃ao ignorados.*/

#include <stdio.h>

struct Produto {
    int codigo;
    char descricao[50];
    float preco;
};

void cadastrarCardapio(struct Produto cardapio[], int n);
void receberPedidos(struct Produto cardapio[], int n);

int main() {
    int n;

    printf("Digite o numero de produtos no cardapio: ");
    scanf("%d", &n);

    struct Produto cardapio[n];

    cadastrarCardapio(cardapio, n);
    receberPedidos(cardapio, n);

    return 0;
}

void cadastrarCardapio(struct Produto cardapio[], int n) {
    printf("Cadastro do cardapio:\n");

    for (int i = 0; i < n; i++) {
        printf("Produto %d:\n", i + 1);
        printf("Codigo: ");
        scanf("%d", &cardapio[i].codigo);
        printf("Descricao: ");
        scanf("%s", cardapio[i].descricao);
        printf("Preco: ");
        scanf("%f", &cardapio[i].preco);
        printf("\n");
    }
}

void receberPedidos(struct Produto cardapio[], int n) {
    printf("Faca seus pedidos (digite 0 para encerrar):\n");

    float totalPagar = 0;

    for (;;) {
        int codigo, quantidade;

        printf("Codigo do produto: ");
        scanf("%d", &codigo);

        if (codigo == 0) {
            break;
        }
        if (codigo < 1 || codigo > n) {
            printf("Produto nao cadastrado. Digite um codigo valido.\n");
            continue;
        }
        printf("Quantidade: ");
        scanf("%d", &quantidade);
        if (quantidade < 0) {
            printf("Quantidade inválida. Digite uma quantidade positiva.\n");
            continue;
        }
        totalPagar += cardapio[codigo - 1].preco * quantidade;
    }

    printf("Total a pagar: %.2f\n", totalPagar);
}