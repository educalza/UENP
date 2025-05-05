/*Implemente uma fun ̧c ̃ao recursiva para calcular a soma dos elementos de um array. Por
exemplo, se o array dado for: int array[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} a resposta deve ser 55*/

#include <stdio.h>

int SomaDeArray(int n, int array[]);

int main(){

    int n;

    printf("Digite o tamanho da array: ");
    scanf("%d", &n);
    int array[n];

    printf("Digite os numeros da array: ");
    for(int i=0; i<n; i++){
        printf("Array[%d]: ", i);
        scanf("%d", &array[i]);
    }

    int func = SomaDeArray(n, array);

    printf("Soma da array: %d", func);

}

int SomaDeArray(int n, int array[]){

    if(n==0){
        return 0;
    }

    return array[n-1] + SomaDeArray(n-1, array);
}