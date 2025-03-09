/*Implemente um programa que leia os c ́odigos de matr ́ıcula dos alunos de duas disciplinas
(progI e progII) e mostre os c ́odigos de matr ́ıculas dos alunos que est ̃ao cursando ambas as
disciplinas. O n ́umero de alunos de ambas as turmas  ́e informado pelo usu ́ario. O espa ̧co
de mem ́oria necess ́ario para armazenar os c ́odigos das matr ́ıculas nas disciplinas deve ser
criado dinamicamente, de acordo com esse n ́umero.*/

#include <stdio.h>
#include <stdlib.h>

int main(){

    int turma1, turma2;

    printf("Digite a quantidade de alunos na turma de Prog 1: ");
    scanf("%d", &turma1);
    printf("Digite a quantidade de alunos na turma de Prog 2: ");
    scanf("%d", &turma2);

    int *matriculaProg1 = (int*)calloc(turma1, sizeof(int));
    int *matriculaProg2 = (int*)calloc(turma2, sizeof(int));

    printf("Digite a matricula dos alunos de Prog1: \n");
    for(int i=0; i<turma1; i++){
        printf("Aluno[%d]: ", i+1);
        scanf("%d", matriculaProg1+i);
    }
    printf("Digite a matricula dos alunos de Prog2: \n");
    for(int i=0; i<turma2; i++){
        printf("Aluno[%d]: ", i+1);
        scanf("%d", matriculaProg2+i);
    }

    printf("Alunos matriculados em ambas as disciplinas: \n");
    for(int i=0; i<turma1; i++){
        for(int j=0; j<turma2; j++){
            if(matriculaProg1[i]==matriculaProg2[j]){
                printf("%d ", matriculaProg1[i]);
            }
        }
    }
    free(matriculaProg1);
    free(matriculaProg2);

    return 0;
}