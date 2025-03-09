/*Faça um programa que receba uma string de no máximo 255 caracteres e, em seguida implemente funções que:

a) Receba como parâmetro a string, calcula e retorna o total de palavras contidas na string.
b) Receba como parâmetro a string e imprime a última palavra da string.
c) Receba como parâmetro a string, calcula retorna a porcentagem de vogais em relação ao total de caracteres na string.
d) Receba como parâmetro a string e dois valores representando duas posições na string, mostre a substring (caracteres) contida entre os dois valores informados pelo usuárioo.
e) Receba como parâmetro a string e o valor de deslocamento 'n' e retorna a string por um caractere deslocado 'n' posições no alfabeto, onde 'n' é um número inteiro fornecido pelo usuário.
Por exemplo, se string = "programa em c" e n=5, então string criptografada = "uwtlwfrf jr h".

OBS: Não usar a biblioteca <string.h>*/

#include <stdio.h>
#define N 255

int aCalculaLetra(char str[]){
    int count = 0;
    while (str[count] != '\0'){
        count++;
    }
    return count;
}

void bImprimeUltimPalavraString(char str[]){
    int count = 0;
    while (str[count] != '\0'){
        count++;
    }
    int countUltimaPalavra;
    for (int i = count; i > 0; i--){
        if (str[i] == ' '){
            countUltimaPalavra = i;
            break;
        }
    }
    for (int i = countUltimaPalavra + 1; i < count; i++){
        printf("%c", str[i]);
    }
}

float cRetornaPorcentagemVogal(char str[]){
    int count=0;
    while(str[count]!='\0'){
        count++;
    }
    char vogal[10] = {'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U'};
    int countVogal=0;
    for(int i=0; i<count; i++){
        for(int j=0; j<10; j++){
            if(str[i] == vogal[j]){
                countVogal++;
            }
        }
    }
    count--; // está contando com o \0
    return (float)(countVogal*100)/count; 
}

void dPrintaSubstringDoUsuario(char str[], int x, int y){
    int count=0;
    while(str[count]){
        count++;
    }
    if(x>0 && x<y && y<count){
        for(int i=x; i<=y; i++){
            printf("%c", str[i]);
        }
    }else{
        printf("Parametro para substring invalido.\n");
    }
}

void eCriptografarString(char str[], int cripto){
    if(cripto<0||cripto>26){
        printf("Numero maior que letras do alfabeto.\n");
    }
    else{
        int count=0;
        while(str[count]!='\0'){
            count++;
        }
        char stringCriptografada[count];
        for(int i=0; i<count; i++){
            if(str[i]==' '){
                stringCriptografada[i] = str[i];
            }else{
                stringCriptografada[i] = str[i] + cripto;
            }
        }
        stringCriptografada[count-1] = '\0';
        puts(stringCriptografada);
    }
}

int main(){

    char str[N];

    printf("Digite a string: ");
    fgets(str, N, stdin);

    printf("Numero de letras na strin: %d\n", aCalculaLetra(str) - 1);
    bImprimeUltimPalavraString(str);
    printf("Porcentagem de vogais comparado com a string inteira: %.2f", cRetornaPorcentagemVogal(str));

    int x, y;
    printf("Digite os numeros para pegar a substring: ");
    printf("\nx: ");
    scanf("%d", &x);
    printf("y: ");
    scanf("%d", &y);
    dPrintaSubstringDoUsuario(str, x, y);
    printf("\n");

    int criptografar;
    printf("Digite um numero para criptografar a string: ");
    scanf("%d", &criptografar);
    eCriptografarString(str, criptografar);
}