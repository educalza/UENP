/*Uma empresa precisa de um banco de dados para armazenar os dados dos usu ́arios de sua
rede social. Inicialmente o programa deve receber a quantidade de usu ́arios que deseja
cadastrar. Em seguida, as seguintes informa ̧c ̃oes devem ser guardadas para cada usu ́ario:*/

#include <stdio.h>

struct BancoDeDadosRedeSocial{
    int idade;
    char nome[30];
    char sexo;
    char estadoCivil;
    int quantidadeDeAmigos;
    int QuantidadesDeFotos;
};

int main(){

    int n=0;

    printf("Digite a quantidade de usuarios a ser registrados: ");
    scanf("%d", &n);
    printf("Digite os dados a seguir. \n");

    struct BancoDeDadosRedeSocial usuario[n];

    for(int i=0; i<n; i++){
        printf("Usuario %d\n", i+1);
        printf("Nome: ");
        setbuf(stdin, NULL);
        fgets(usuario[i].nome, 30, stdin);
        printf("Idade: ");
        scanf("%d", &usuario[i].idade);
        printf("Sexo(m - masculino, f - feminino): ");
        scanf(" %c", &usuario[i].sexo);
        printf("Estado Civil(s - solteiro, c - casado, n - namorando, d - divorciado): ");
        scanf(" %c", &usuario[i].estadoCivil);
        printf("Quantidade de amigos: ");
        scanf("%d", &usuario[i].quantidadeDeAmigos);
        printf("Quantidade de fotos: ");
        scanf("%d", &usuario[i].QuantidadesDeFotos);
        printf("\n\n");
    }

    printf("----Banco de dados dos usuarios----\n");
    for(int i=0; i<n; i++){
        printf("Nome: %s", usuario[i].nome);
        printf("Idade: %d\n", usuario[i].idade);
        printf("Sexo: ");
        if(usuario[i].sexo=='m'){
            printf("Masculino.\n");
        }
        else if(usuario[i].sexo=='f'){
            printf("Feminino.\n");
        }

        printf("Estado Civil:");
        if(usuario[i].estadoCivil=='s'){
            printf("Solteiro.\n");
        }
        if(usuario[i].estadoCivil=='c'){
            printf("Casado.\n");
        }
        if(usuario[i].estadoCivil=='n'){
            printf("Namorando.\n");
        }
        if(usuario[i].estadoCivil=='d'){
            printf("Divorciado.\n");
        }
        printf("Quantidade de amigos: %d\n", usuario[i].quantidadeDeAmigos);
        printf("Quantidade de fotos: %d\n", usuario[i].QuantidadesDeFotos);
        printf("\n\n");
    }
    return 0;
}