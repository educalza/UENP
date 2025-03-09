// ler coordenadas e medir a distância entre elas
// distância entre coordenadas d(A, B) = raiz(x² − x)² + (y² − y)²
// portanto   d(A, B)
#include <stdio.h>
#include <math.h> // inclui algumas propriedades da matemática, usaremos raiz e expoente

int main(){

    int x1, x2, y1, y2;

    printf("Digite as coordenadas de A no plano cartesiano:");
    scanf("%d%d", &x1, &y1);

    printf("Digite as coordenadas de B no plano cartesiano:");
    scanf("%d%d", &x2, &y2);

    float distancia;
    distancia = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2)); // pow (2, 2) - o uso de vários parênteses se da pela subtração, que tem que ser resolvida antes de elevar ao expoente

    printf("Distancia entre os porntos: %.2f", distancia);

    return 0;
}