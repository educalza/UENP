/*Fa ̧ca um programa que leia uma matriz quadrada de ordem 4 X 4 de n ́umeros inteiros.
Depois, leia um n ́umero x e verifique quantas vezes x aparece na matriz.*/

#include <stdio.h>
#define L 4
#define C 4

int main(){

    int matriz[L][C];
    int *ptr = &matriz[0][0];

    printf("Digite os numeros da matriz: \n");
    for(int i=0; i<L*C; i++){
        printf("Matriz: ");
        scanf("%d", *(ptr+1));
    }

    int x=0, count=0;
    printf("Digite um numero qualquer: ");
    scanf("%d", &x);

    for(int i=0; i<L*C; i++){
        if(*(ptr+1)==x){
            count++;
        }
    }
    printf("%d apareceu %d vezes: ", x, count);

    return 0;
}