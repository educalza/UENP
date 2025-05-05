/*Implemente uma fun ̧c ̃ao recursiva respons ́avel por converter um numero n na base decimal
para bin ́aria.*/
/*decimal -> binario*/

#include <stdio.h>
#include <stdlib.h>

void converterDecimalBinario(int n);

int main(){

    int n;

    printf("Digite um decimal: ");
    scanf("%d", &n);

    converterDecimalBinario(n);

    return 0;
}

void converterDecimalBinario(int n){

    
    if(n==0){
        return;
    }
    else{
        converterDecimalBinario(n/2);
        printf("%d", n%2);
    }
}
