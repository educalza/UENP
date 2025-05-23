#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define TAMAX 5

    // Funcao para misturar o vetor
void mistureVetor(int arr[], int n){
        for (int i = 0; i < n; i++){
            if ((rand() / (double)RAND_MAX) < 0.5){
                int j = (int)((n - 1) * (rand() / (double)RAND_MAX));
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }

int main(){

    int tamanhos[] = {5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);
    
    // Abrir arquivo para armazenar os resultados
    FILE * fp_selection = fopen ("MetodoDeOrdenacao" , "w" );
    srand ( time (0) ) ; // Iniciar os numeros aleatorios
    
    // Loop pelos tamanhos de vetor
    for (int i = 0; i < num_tamanhos ; i ++) {
        int tam = tamanhos[i];
        int *arr = (int *)malloc(tam * sizeof(int));
        // Preencher o array com valores crescentes
        for (int j = 0; j < tam; j++){
            arr[j] = j;
        }
        // Executar 30 repeticoes para obter o caso medio
        for (int j = 0; j < 30; j++){
            mistureVetor(arr, tam);
            int iteracoes;
            // Selection Sort
            /*Método de ordenação*/(arr, tam, &iteracoes);
            fprintf(fp_selection, "%d", iteracoes);
        }
        fprintf(fp_selection, "\n");
        free(arr); // Liberar memoria do array
    }
    // Fechar arquivo
    fclose ( fp_selection );
    return 0;
    }