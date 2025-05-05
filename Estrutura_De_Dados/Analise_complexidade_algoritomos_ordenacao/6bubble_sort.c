#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função para misturar o vetor usando Fisher-Yates shuffle
void mistureVetor(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

// Implementação do Bubble Sort com contagem de iterações
void bubbleSort(int arr[], int n, int *iteracoes) {
    *iteracoes = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            (*iteracoes)++;
            if (arr[j] > arr[j + 1]) {
                // Troca os elementos
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    // Todos os 10 tamanhos de array
    int tamanhos[] = {5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    // Abrindo os arquivos para armazenar os resultados no modo de escrita
    FILE *fp_caso_medio = fopen("BubbleSortIteracoesCasoMedio.txt", "w");
    FILE *fp_pior_caso = fopen("BubbleSortIteracoesPiorCaso.txt", "w");
    if (fp_caso_medio == NULL || fp_pior_caso == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    srand(time(0)); // Iniciando os números aleatórios

    for (int i = 0; i < num_tamanhos; i++) {
        int tam = tamanhos[i];
        int *arr_caso_medio = (int *)malloc(tam * sizeof(int));
        int *arr_pior_caso = (int *)malloc(tam * sizeof(int));

        // Preenchendo o vetor com valores crescentes para o caso médio
        for (int j = 0; j < tam; j++) {
            arr_caso_medio[j] = j;
        }

        // Preenchendo o vetor com valores decrescentes para o pior caso
        for (int j = 0; j < tam; j++) {
            arr_pior_caso[j] = tam - j;
        }

        // Executando 30 repetições para obter o caso médio
        for (int j = 0; j < 30; j++) {
            mistureVetor(arr_caso_medio, tam);
            int iteracoes_medio;
            // Bubble Sort para caso medio
            bubbleSort(arr_caso_medio, tam, &iteracoes_medio);
            fprintf(fp_caso_medio, "%d ", iteracoes_medio); // Gravando no arquivo de saída o total de iterações para cada uma das 30 repetições
        }
        fprintf(fp_caso_medio, "\n");

        // Executando 30 repetições para obter o pior caso
        for (int j = 0; j < 30; j++) {
            int iteracoes_pior;
            // Bubble Sort para pior caso
            bubbleSort(arr_pior_caso, tam, &iteracoes_pior);
            fprintf(fp_pior_caso, "%d ", iteracoes_pior); // Gravando no arquivo de saída o total de iterações para cada uma das 30 repetições
        }
        fprintf(fp_pior_caso, "\n");

        // Desalocando a memória dos vetores
        free(arr_caso_medio);
        free(arr_pior_caso);

        // Impressão de progresso
        printf("Processamento concluido para tamanho do vetor: %d\n", tam);
    }

    // Fechando os arquivos de saída
    fclose(fp_caso_medio);
    fclose(fp_pior_caso);

    printf("Processamento completo!\n");

    return 0;
}
