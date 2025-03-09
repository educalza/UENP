/*Uma pesquisa feita com alunos do ensino fundamental coletou alguns dados para an ́alise,
como nome, idade, altura e peso. Fa ̧ca um programa que leia os dados de 20 crian ̧cas e
armazene em um vetor de estrutura. Depois, implemente uma fun ̧c ̃ao que mostre o nome
do aluno mais novo e o nome do aluno com o maior peso.*/

#include <stdio.h>
#include <string.h>
#define C 20

struct AnaliseCriancas {
    char nome[40];
    int idade;
    float altura;
    float peso;
};

    struct AnaliseCriancas dados[C];

    void AlunoMaisNovo(struct AnaliseCriancas dados[]);
    void AlunoComMaiorPeso(struct AnaliseCriancas dados[]);

int main(){


    for(int i=0; i<C; i++){
        printf("Nome: ");
        setbuf(stdin, NULL);
        fgets(dados[i].nome, 40, stdin);
        printf("Idade: ");
        scanf("%d", &dados[i].idade);
        printf("Altura: ");
        scanf("%f", &dados[i].altura);
        printf("Peso: ");
        scanf("%f", &dados[i].peso);
        printf("\n");
    }

    AlunoMaisNovo(dados);
    printf("\n\n");
    AlunoComMaiorPeso(dados);

}

void AlunoMaisNovo(struct AnaliseCriancas dados[]){

    printf("Aluno(s) de menor idade: \n");

    int menorIdade = dados[0].idade;

    for(int i=0; i<C; i++){
        if(dados[i].idade<menorIdade){
            menorIdade = dados[i].idade;
        }
    }
    
    for(int i=0; i<C; i++){
        if(dados[i].idade==menorIdade){
            printf("%.*s ", (int)strcspn(dados[i].nome, "\n"), dados[i].nome);
            printf("%d anos\n", dados[i].idade);
        }
    }
}

void AlunoComMaiorPeso(struct AnaliseCriancas dados[]){

    int maiorPeso = dados[0].peso;

    for(int i=0; i<C; i++){
        if(dados[i].peso>maiorPeso){
            maiorPeso = dados[i].peso;
        }
    }
    printf("Aluno(s) com maior peso: \n");
    for(int i=0; i<C; i++){
        if(dados[i].peso==maiorPeso){
            printf("%.*s ", (int)strcspn(dados[i].nome, "\n"), dados[i].nome);
            printf("%.2f kg\n", dados[i].peso);
        }
    }
}