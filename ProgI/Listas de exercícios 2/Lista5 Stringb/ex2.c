/*Fa ̧ca um programa que leia uma matriz de 5 strings e imprima cada uma delas. Depois,
pe ̧ca para o usu ́ario digitar mais uma string e verifique se ela est ́a na matriz, caso esteja
indique em que linha ela se encontra. (SEM USAR STRING.H)*/

#include <stdio.h>
#define N 50

int main() {

    char matriz[5][N];
    char busca[N];
    int i, h = 0, j, f = 0;

    for(int i=0; i<5; i++){
    printf("Digite 5 strings %d: ", i+1);
    fgets(matriz[i], N, stdin);
    }

    for (i=0; i<5; i++) {
            printf("%s\n", matriz[i]);
        }

    printf("Digite uma string para verificar se ela esta na matriz: ");
    scanf("%s", busca);

    int count = 0;

    while (busca[count] != '\0') {
        count++;
    }

        for (j = 0; j < 5; j++) {
        h = 0;
        for (i = 0; i < count; i++) {
            if (matriz[j][i] != busca[i]) {
                h++;
                break;
            }
        }
        if (h == 0) {
            printf("A string '%s' foi encontrada na linha %d.\n", busca, j + 1);

        }
    }
    if(f == 5){
        printf("A string '%s' nao esta na matriz.\n", busca);
    }
    return 0;
}