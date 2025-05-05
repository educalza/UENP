/*Fa ̧ca um programa em leia uma sequˆencia de n ́umeros e armazene em um vetor. Em
seguida, determine e mostre quantos n ́umeros lidos est ̃ao dentro do intervalo entre 10 e 50.
O programa deve ser encerrado quando for lido o n ́umero -1.*/

#include <stdio.h>

int main(){

    int i=0, count=0;
    int vetor[100000] = {0};

    printf("Digite quantos numeros quiser (digite -1 para parar)\n");

    for(i=0; i==i; i++){
        printf("vetor[%d]: ", i);
        scanf("%d", &vetor[i]);
        if(vetor[i]>10 && vetor[i]<50){
            count++;
        }
        if(vetor[i]==-1){
            break;
        }
    }
    printf("\n");
    printf("Numeros entre 10 e 50 apareceu %d vezes.\n", count);
    printf("Nos respectivos vetores: \n");
    for(i=0; i==i; i++){
        if(vetor[i]>10 && vetor[i]<50){
            printf("vetor[%d]: %d \n", i, vetor[i]);
        }
        if(vetor[i]==-1){
            break;
        }
    }
    return 0;
}