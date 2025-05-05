/*Dada a estrutura a seguir representando as coordenadas de um ponto no sistema cartesiano.
Implemente uma fun ̧c ̃ao que, dado dois pontos fornecidos pelo usu ́ario, calcule a distˆancia
entre eles.*/

#include <stdio.h>
#include <math.h>

int main(){

    struct coordenadas{
        float x;
        float y;
    };
    struct coordenadas ponto[2];

    for(int i=0; i<2; i++){
    printf("Digite as coordendas do ponto %d.\n", i+1);
    printf("x: ");
    scanf("%f", &ponto[i].x);
    printf("y: ");
    scanf("%f", &ponto[i].y);
    printf("\n");
    }

    float distancia;
    distancia = sqrt(pow((ponto[0].x - ponto[1].x),2)+pow((ponto[0].y - ponto[1].y),2));

    printf("Distancia entre os pontos (%.2f,%.2f) e (%.2f,%.2f): %.2f", ponto[0].x, ponto[0].y, ponto[1].x, ponto[1].y, distancia);

    return 0;
}