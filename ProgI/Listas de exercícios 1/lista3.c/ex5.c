/*Fa ̧ca um programa que receba uma quantidade de n ́umeros inteiros, em seguida calcule e
mostre a sua m ́edia. A quantidade de n ́umeros deve ser fornecida pelo usu ́ario.*/

#include <stdio.h>

int main(){

    int num, numeros, soma = 0;
    float media;

    printf("Digite quantos numeros serao calculados: \n");
    scanf("%d", &num);
    printf("Agora digite os numeros para realizar a media: \n");

    for(int i = 0; i<num; i++){
        printf("Numero[%d]: ", i+1);
        scanf("%d", &numeros);
        soma += numeros;
    }
    media = soma/num;
    printf("Valor da media dos numeros digitados: %2.f", media);

    return 0;
}