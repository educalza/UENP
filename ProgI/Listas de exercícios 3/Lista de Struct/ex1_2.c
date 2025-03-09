/*Dada a estrutura a seguir representando as coordenadas de um ponto no sistema cartesiano.
Implemente uma fun ̧c ̃ao que, dado dois pontos fornecidos pelo usu ́ario, calcule a distˆancia
entre eles.*/

#include <stdio.h>
#include <math.h>

struct Ponto{
    float x;
    float y;
};

int main(){

    struct Ponto coordenada1;
    struct Ponto coordenada2;
    float DistanciaEntrePontos(struct Ponto c1, struct Ponto c2);

    printf("Digite as coordenadas de x e y.\n");

    printf("x1: ");
    scanf("%f", &coordenada1.x);
    printf("y1: ");
    scanf("%f", &coordenada1.y);
    printf("\n");
    printf("x2: ");
    scanf("%f", &coordenada2.x);
    printf("y2: ");
    scanf("%f", &coordenada2.y);

    float func;
    func = DistanciaEntrePontos(coordenada1, coordenada2);
    printf("Distancia entre os pontos: %.2f", func);
}

float DistanciaEntrePontos(struct Ponto c1, struct Ponto c2){

    float distancia=0;
    
    distancia = sqrt(pow((c1.x-c2.x),2)+pow((c1.y-c2.y),2));
    return distancia;
}