/*Implemente fun ̧c ̃oes para:
a. Ler os dados de no m ́aximo 50 alunos e amarzene em um vetor de estrutura.
b. Exibir os dados dos alunos, informando os alunos aprovados (nota ≥ 7.0), de exame
(4.0 ≤ nota < 7.0) e reprovados (nota < 4.0).
c. Mostrar qual  ́e a m ́edia geral da turma.
d. Exibir o nome do aluno com a maior nota da turma.*/

#include <stdio.h>
#include <string.h>
#define A 2

struct cadastroAluno{
    int matricula;
    char nome[50];
    float nota;
};

void aCadastrarAlunos(struct cadastroAluno cadas[]);
void bExibirDados_ReprovadosAprovados(struct cadastroAluno cadas[]);
void cMediaGeralDaTurma(struct cadastroAluno cadas[]);
void dAlunoDeMaiorNotaDaTurma(struct cadastroAluno cadas[]);

int main(){

    struct cadastroAluno aluno[A];

    aCadastrarAlunos(aluno);
    system("cls");
    bExibirDados_ReprovadosAprovados(aluno);
    cMediaGeralDaTurma(aluno);
    dAlunoDeMaiorNotaDaTurma(aluno);

}

void aCadastrarAlunos(struct cadastroAluno cadas[]){


    printf("----Cadastro de alunos----\n\n");
    for(int i=0; i<A; i++){
        printf("Matricula: ");
        scanf("%d", &cadas[i].matricula);
        printf("Nome: ");
        setbuf(stdin, NULL);
        fgets(cadas[i].nome, 50, stdin);
        printf("Nota: ");
        scanf("%f", &cadas[i].nota);
        printf("\n");
    }
}

void bExibirDados_ReprovadosAprovados(struct cadastroAluno cadas[]){

    for(int i=0; i<A; i++){
        printf("Matricula: %d\n", cadas[i].matricula);
        printf("Nome: %s", cadas[i].nome);
        printf("Nota: %.2f\n", cadas[i].nota);
        if(cadas[i].nota>=70){
            printf("Aluno APROVADO!\n");
        }
        else if(cadas[i].nota>=40 && cadas[i].nota<70){
            printf("Aluno em EXAME!\n");
        }
        else if(cadas[i].nota<40){
            printf("Aluno REPROVADO!\n");
        }
        printf("\n");
    }
}

void cMediaGeralDaTurma(struct cadastroAluno cadas[]){

    float media=0, soma=0;

    for(int i=0; i<A; i++){
        soma += cadas[i].nota;
    }
    media = soma/A;
    printf("Media geral da turma: %.2f\n", media);
}

void dAlunoDeMaiorNotaDaTurma(struct cadastroAluno cadas[]){

    float MaiorNota = cadas[0].nota;
    int countAluno = 0;

    for(int i=0; i<A; i++){
        if(cadas[i].nota>MaiorNota){
            MaiorNota = cadas[0].nota;
            countAluno = i;
        }
    }
    printf("Aluno de maior nota: %s", cadas[countAluno].nome);
}