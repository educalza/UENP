/*Implemente um programa em C leia quatro valores inteiros A, B, C e D. A seguir, calcule
e mostre a diferen ̧ca do produto de A e B pelo produto de C e D.*/

#include <stdio.h>

int main(){

    int A=0, B=0, C=0, D=0;

    printf("Digite os valores inteiros para: \n");

    printf("A- ");
    scanf("%d", &A);

    printf("B- ");
    scanf("%d", &B);

    printf("C- ");
    scanf("%d", &C);

    printf("D- ");
    scanf("%d", &D);

    int produtoAB, produtoCD, DiferencaProduto;
    produtoAB = A*B;
    produtoCD = C*D;
    DiferencaProduto = produtoAB - produtoCD;

    printf("Diferença do produto de A e B pelo C e D: %d", DiferencaProduto);

    return 0;
}