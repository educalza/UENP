#include <stdio.h>

int main(){

    int vet[10];
    int impar[10] = {0};
    int par[10] = {0};

    for(int i = 0; i <10;i++){
        scanf("%d", &vet[i]);
        if(vet[i] % 2 == 0){
            par[i] = vet[i];
        }
        else{
        impar[i] = vet[i];
        }
    }

    printf("Valores pares:\n");
    for(int i = 0; i < 10;i++){
        if(par[i] != 0){
            printf("%d\n", par[i]);
        }
    }

    printf("\n");

    printf("Valores impares:\n");
    for(int i = 0; i < 10;i++){
        if(impar[i] != 0){
            printf("%d\n", impar[i]);
        }
    }
    return 0;
}