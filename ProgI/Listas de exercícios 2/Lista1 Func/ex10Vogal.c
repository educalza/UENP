//Fa ̧ca uma fun ̧c ̃ao que receba um caractere e verifique se ele  ́e uma vogal ou n ̃a

#include <stdio.h>

int main(){
    
    char letra, m;
    int vogalOuNao(char caracter);
    
    printf("Digite uma letra para verificar se ela eh vogal ou nao: ");
    scanf("%c", &letra);
    
    m = vogalOuNao(letra);
    

}

int vogalOuNao(char caracter){
    if(caracter == 'A' || caracter == 'E' || caracter == 'I' || caracter == 'O' || caracter == 'U' || caracter == 'a' || caracter == 'e' || caracter == 'i' || caracter == 'o' || caracter == 'u'){
        printf("A letra digitada eh uma vogal");
    }
    else{
        printf("A letra digitada nao eh uma vogal");
    }
    return caracter;
}
