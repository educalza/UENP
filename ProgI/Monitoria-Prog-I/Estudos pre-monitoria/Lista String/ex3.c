/*Fa¸ca um programa que leia uma string informada pelo usu´ario e, em seguida, implemente
uma fun¸c˜ao que contabilize a quantidade de espa¸cos em branco existente na string lida.*/

#include <stdio.h>
#define N 255

int main(){

    char str[N];
    printf("Digite a frase: ");
    fgets(str, N, stdin);

    int count=0;
    while(str[count] != '\0'){
        count++;
    }

    int countEspaco=0;
    for(int i=0; i<count; i++){
        if(str[i] == ' '){
            countEspaco++;
        }
    }

    printf("A frase tem o total de %d espacos.\n", countEspaco);

    return 0;
}