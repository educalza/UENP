#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função para misturar o vetor
void mistureVetor(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        if ((rand() / (double)RAND_MAX) < 0.5) {
            int j = (int)((n - 1) * (rand() / (double)RAND_MAX));
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
}

// Função para trocar dois elementos
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

// Função de partição usada no Quick Sort
int partition(int arr[], int low, int high, int *iteracoes) {
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        (*iteracoes)++;
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Função de partição randomizada
int randomPartition(int arr[], int low, int high, int *iteracoes) {
    int randomIndex = low + rand() % (high - low + 1);
    swap(&arr[randomIndex], &arr[high]);
    return partition(arr, low, high, iteracoes);
}

// Implementação do Quick Sort Randomizado
void quickSortRandomizado(int arr[], int low, int high, int *iteracoes) {
    if (low < high) {
        int pi = randomPartition(arr, low, high, iteracoes);

        quickSortRandomizado(arr, low, pi - 1, iteracoes);
        quickSortRandomizado(arr, pi + 1, high, iteracoes);
    }
}

int main() {
    int tamanhos[] = {5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    // Abrir arquivos para armazenar os resultados
    FILE *fp_quick_medio = fopen("QuickSortRandomizadoIteracoesCasoMedio.txt", "w");
    FILE *fp_quick_pior = fopen("QuickSortRandomizadoIteracoesCasoPior.txt", "w");
    srand(time(0)); // Iniciar os números aleatórios

    // Loop pelos tamanhos de vetor
    for (int i = 0; i < num_tamanhos; i++) {
        int tam = tamanhos[i];
        int *arr = (int *)malloc(tam * sizeof(int));

        // Executar 30 repetições para obter o caso médio
        for (int j = 0; j < 30; j++) {
            // Preencher o array com valores crescentes
            for (int k = 0; k < tam; k++) {
                arr[k] = k;
            }

            mistureVetor(arr, tam);
            int iteracoes = 0;
            // Quick Sort Randomizado
            quickSortRandomizado(arr, 0, tam - 1, &iteracoes);
            fprintf(fp_quick_medio, "%d ", iteracoes);
        }
        fprintf(fp_quick_medio, "\n");

        // Calcular o pior caso (array em ordem crescente, pois o Quick Sort tende a ter pior desempenho com arrays já ordenados)
        for (int j = 0; j < 30; j++) {
            // Preencher o array com valores crescentes
            for (int k = 0; k < tam; k++) {
                arr[k] = k;
            }
            int iteracoes_pior = 0;
            // Quick Sort Randomizado
            quickSortRandomizado(arr, 0, tam - 1, &iteracoes_pior);
            fprintf(fp_quick_pior, "%d ", iteracoes_pior);
        }
        fprintf(fp_quick_pior, "\n");

        free(arr); // Liberar memória do array
         printf("Processamento concluido para tamanho do vetor: %d\n", tam);

    }

    // Fechar arquivos
    fclose(fp_quick_medio);
    fclose(fp_quick_pior);

    return 0;
}
