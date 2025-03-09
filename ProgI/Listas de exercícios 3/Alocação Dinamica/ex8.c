/*Defina uma estrutura aluno contendo os campos nome, sobrenome e nota. O programa
deve ler esses dados e armazenar dinamicamente na mem ́oria. Depois, implemente uma
fun ̧c ̃ao para imprimir o nome e sobrenome do aluno que possui maior nota. O prot ́otipo
da fun ̧c ̃ao  ́e seguinte:
void alunoMaiorNota(struct aluno *vetAlunos, int tam) {..}*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct DadosAlunos{
    char nome[30];
    char sobrenome[30];
    float nota;
};

void lerdados(struct DadosAlunos *aluno, int n);
void alunoMaiorNota(struct DadosAlunos *vetAlunos, int tam);

int main(){

    int n;

    printf("Digite quantos alunos serao analisados: ");
    scanf("%d", &n);

    struct DadosAlunos *aluno;
    aluno = (struct DadosAlunos*)calloc(n, sizeof(struct DadosAlunos));

    lerdados(aluno, n);
    printf("\n");
    printf("Aluno(s) com maior(res) nota(s).\n");
    alunoMaiorNota(aluno, n);
}

void lerdados(struct DadosAlunos *aluno, int n){

    for(int i=0; i<n; i++){
        printf("Nome: ");
        setbuf(stdin, NULL);
        fgets(aluno[i].nome, 30, stdin);
        printf("Sobrenome: ");
        setbuf(stdin, NULL);
        fgets(aluno[i].sobrenome, 30, stdin);
        printf("Nota: ");
        scanf("%f", &aluno[i].nota);
        printf("\n");
    }
}

void alunoMaiorNota(struct DadosAlunos *vetAlunos/*aluno*/, int tam/*n*/){

    int maiorNota = vetAlunos[0].nota;

    for(int i=0; i<tam; i++){
        if(vetAlunos[i].nota>maiorNota){
            maiorNota = vetAlunos[i].nota; 
        }
    }
    for(int i=0; i<tam; i++){
        if(maiorNota==vetAlunos[i].nota){
            printf("%.*s ", strcspn(vetAlunos[i].nome, "\n"), vetAlunos[i].nome);
            printf("%s", vetAlunos[i].sobrenome);
        }
    }
}