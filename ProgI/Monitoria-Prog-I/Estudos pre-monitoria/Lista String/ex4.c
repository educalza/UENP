/*Fa¸ca um programa que leia duas strings quaisquer e mostre se as duas s˜ao iguais ou
diferentes no conte´udo e se possuem o mesmo tamanho. Lembre-se que as strings podem
conter letras mai´usculas e/ou min´usculas.*/

#include <stdio.h>
#define N 255

int main(){

    char str[N];
    printf("Digite a primeira string: ");
    fgets(str, N, stdin);
    puts(str);

    char str2[N];
    printf("Digite a segunda string: ");
    fgets(str2, N, stdin);
    puts(str2);

    int count1=0;
    int count2=0;
    while(str[count1] != '\0'){
        count1++;
    }
    while(str2[count2] != '\0'){
        count2++;
    }

    if(count1 != count2){
        printf("As strings possuem tamanhos diferentes, logo sao diferentes.\n");
    }else{
        int stringCorreta=0;
        for(int i=0; i<count1-1; i++){
            if(str[i] == str2[i]){
                stringCorreta = 1;
            }
            stringCorreta =0;
        }
        if(stringCorreta == 1){
            printf("As strings sao iguais.\n");
        } else if(stringCorreta == 0){
            printf("As strings sao diferentes.\n");
        }
    }

    return 0;
}
