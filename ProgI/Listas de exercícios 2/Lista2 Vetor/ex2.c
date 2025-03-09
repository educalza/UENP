/*Fa ̧ca um programa que receba do usu ́ario um vetor de “n” n ́umeros inteiros e, em seguida,
imprima o vetor na ordem inversa.*/

#include <stdio.h>

int main(){

    int n;

    printf("Digite a quantidade de numeros a ser armazenada: ");
    scanf("%d", &n);

    int cuzinho[n];

    for(int i=0; i<n; i++){
        printf("vetor[%d]: ", i);
        scanf("%d", &cuzinho[i]);
    }

    printf("\n");

    for(int i=n-1; i>=0; i--){
        printf("vetor[%d]: %d\n", i, cuzinho[i]);
    }
    return 0;
}