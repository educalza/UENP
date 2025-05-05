/*Uma maquina automatica de cafe so funciona com notas de R$ 5 e nunca da o troco. O
cafe custa R$ 7. Assim, se o cliente pedir apenas um cafe, ele vai perder R$ 3, pois tera
que dar R$ 10, e a maquina nao devolvera o troco. E possıvel comprar mais de um cafe em uma so compra.
Logo, se o cliente colocou R$ 15, significa que ele vai obter dois cafes e
perdera R$ 1. Faca um programa que leia o valor que o cliente colocou na maquina (sempre
multiplo de 5) e imprima quanto o cliente ira perder no troco.*/
#include <stdio.h>
#include <math.h>

int main(){

    int valorInserido = 0, valorCafe = 7, quantidadeCafe, trocoPerdido = 0;

    printf("Digite o valor inserido na maquina: ");
    scanf("%d", &valorInserido);

    quantidadeCafe = valorInserido / valorCafe;
    trocoPerdido = valorInserido - (quantidadeCafe * valorCafe);

    printf("Quantidade de copos de cafe: %d\n", quantidadeCafe);
    printf("Troco perdido: %d", trocoPerdido);

    return 0;
}