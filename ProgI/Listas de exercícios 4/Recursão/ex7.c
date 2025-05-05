/*Modifique o exerc ́ıcio 6 de modo que a fun ̧c ̃ao recursiva calcule a soma dos elementos do
array come ̧cando do  ́ultimo elemento at ́e o primeiro.*/

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