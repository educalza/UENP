/*2. Fa ̧ca um programa para criptografar uma string qualquer informada pelo usu ́ario, substituindo
cada letra da string pela letra do alfabeto em sua respectiva posi ̧c ̃ao, mas de tr ́as para frente.
Por exemplo, se a string digitada for adoro programar em c a sa ́ıda deve ser zwlil kiltiznzi
vn x.
OBS. Considere apenas letras min ́usculas e desconsidere caracteres especiais (*, #, @, etc.).*/

#include <stdio.h>
#include <string.h>
#define N 50
#define ALFABETO 26

int main(){

    char alfabeto[ALFABETO], alfabetoDeTras[ALFABETO];
    char string[N];
    int tamanho =0;

    for(char letra = 'a'; letra<='z'; letra++){
        alfabeto[letra-'a'] = letra;
    }
    for(char letra = 'z'; letra>='a'; letra--){
        alfabetoDeTras['z' - letra] = letra;
    }
    
    char variavelTemp[26];
    strcpy(variavelTemp, alfabeto);

    printf("Digite: \n");
    fgets(string, 50, stdin);
    tamanho = strlen(string);

    for(int i=0; i<tamanho; i++){
        for(int j=0; j<=26; j++){
            if(alfabeto[j]==string[i]){
                string[i] = alfabetoDeTras[j];
                break;
            }
        }
    }
    printf("String criptografada: %s\n", string);
    
    return 0;
}