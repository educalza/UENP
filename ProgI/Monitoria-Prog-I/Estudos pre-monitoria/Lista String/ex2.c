/*Fa¸ca um programa para criptografar uma string qualquer informada pelo usu´ario, substituindo
cada letra da string pela letra do alfabeto em sua respectiva posi¸c˜ao, mas de tr´as para frente.
Por exemplo, se a string digitada for adoro programar em c a sa´ıda deve ser zwlil kiltiznzi
vn x.*/

#include <stdio.h>
#include <string.h>
#define N 26
#define MAX 255

int main()
{

    char str[MAX];

    printf("Digite uma frase: ");
    fflush(stdin);
    fgets(str, MAX, stdin);

    char strCriptografada[MAX];
    int count=0;
    for(int i=0; str[i]!='\0'; i++){
        if(str[i] >= 'a' && str[i] <= 'z'){
            int aux = str[i]-'a';
            strCriptografada[i] = 'z'-aux;
        }
        else if(str[i] >= 'A' && str[i] <= 'Z'){
            int aux = str[i]-'A';
            strCriptografada[i] = 'Z'-aux;
        }
        else{
            strCriptografada[i] = str[i];
        }
        count++;
    }

    strCriptografada[count] = '\0';
    puts(strCriptografada);

    return 0;
}