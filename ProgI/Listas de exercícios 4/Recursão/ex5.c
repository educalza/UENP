/*Implemente uma fun ̧c ̃ao recursiva que receba dois n ́umeros inteiros x e n e calcule o valor
de x^n.*/

#include <stdio.h>

int PotenciaDeNumero(int x, int n);

int main(){

    int x, n;

    printf("Digite um numero: ");
    scanf("%d", &x);
    printf("Digite a potencia: ");
    scanf("%d", &n);

    int func = PotenciaDeNumero(x, n);

    printf("Resultado da potencia: %d", func);

}

int PotenciaDeNumero(int x, int n){

    if(n==0){
        return 1;
    }

    return x * PotenciaDeNumero(x, n-1);
}