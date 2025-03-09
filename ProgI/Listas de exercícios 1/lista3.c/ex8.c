/*Fa ̧ca um programa para calcular e gerar a tabuada dos n ́umeros 1 ao 10.*/

#include <stdio.h>

int main(){

    int num, tabuada;

    printf("Digite a base da sua tabuada: ");
    scanf("%d", &num);

    for(int i=1; i<=10; i++){
        tabuada = i*num;
        printf("%dx%d = %d\n", num, i, tabuada);
    }
    return 0;
}