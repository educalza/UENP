/*Fa ̧ca um programa que receba um vetor de 20 elementos inteiros, em seguida, percorra o
vetor atrav ́es do ponteiro ptr-inicio – a partir do in ́ıcio do vetor e outro ponteiro ptr-fim
a partir do final do vetor, at ́e os dois ponteiros se encontrarem no meio do vetor.*/
#include <stdio.h>
#define N 20

int main(){

    int vetor[N] = {0};

    for(int i=0; i<N; i++){
        printf("Vetor[%d]: ", i);
        scanf("%d", &vetor[i]);
    }

    int *ptr_inicio = vetor, *ptr_fim = vetor+19;

    for(int i=0; i<10; i++){
        printf("%d ", ptr_inicio[i]);
    }

    printf("\n");

    for(int i=19; i>=9; i--){
        printf("%d ", *(ptr_fim--));
    }

    return 0;
}