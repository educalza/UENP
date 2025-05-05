/*Fa ̧ca um programa para calcular e gerar a tabuada dos n ́umeros 1 ao 10.*/

#include <stdio.h>

int main(){

    int num, count = 0;

    printf("Digite um numero para conferir se ele eh primo ou nao: \n");
    scanf("%d", &num);

    for(int i = 1; i<=num; i++){
        if(num%i==0){
            count++;
        }
    }
    if(count == 2){
        printf("Esse numero eh primo");
    }else{
        printf("Essse numero nao eh primo");
    }
    return 0;
}
