/*Fa¸ca um programa que inverta uma string lida. Por exemplo, se a string digitada for
programar em linguagem C, o resultado deve ser C megaugnil me ramargorp.*/

#include <stdio.h>
#define N 250

int main(){

    char str[N];

    printf("Digite a string: ");
    setbuf(stdin, NULL);
    fgets(str, N, stdin);

    int count=0;
    for(int i=0; i<N; i++){
        if(str[i] == '\0'){
            break;
        }
        count++;
    }

    for(int i=count; i>=0; i--){
        printf("%c", str[i]);
    }

    return 0;
}