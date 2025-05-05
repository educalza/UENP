/*A multiplica ̧c ̃ao de dois n ́umeros inteiros pode ser feita atrav ́es de somas sucessivas. Implemente
uma fun ̧c ̃ao recursiva que calcule a multiplica ̧c ̃ao de dois inteiros usando essa ideia.*/

#include <stdio.h>

int ProdutoDaMultiplicacao(int x, int y);

int main(){

    int x, y;

    printf("Digite o multiplicando: ");
    scanf("%d", &x);
    printf("Digite o multiplicador: ");
    scanf("%d", &y);

    int func = ProdutoDaMultiplicacao(x, y);

    printf("Produto: %d", func);

}

int ProdutoDaMultiplicacao(int x, int y){

    if(y==0){
        return 0;
    }

    return x + ProdutoDaMultiplicacao(x, y-1);
}