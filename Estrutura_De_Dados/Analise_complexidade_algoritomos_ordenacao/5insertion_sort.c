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

// Implementação do Insertion Sort com contagem de iterações
void insertionSort(int arr[], int n, int *iteracoes) {
    *iteracoes = 0;

    for (int i = 1; i < n; i++) {
        int chave = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > chave) {
            (*iteracoes)++;
            arr[j + 1] = arr[j];
            j--;
        }
        (*iteracoes)++;
        arr[j + 1] = chave;
    }
}

int main() {
    // Todos os 10 tamanhos de array
    int tamanhos[] = {5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000};
    int num_tamanhos = sizeof(tamanhos) / sizeof(tamanhos[0]);

    // Abrindo os arquivos para armazenar os resultados
    FILE *fp_caso_medio = fopen("InsertionSortIteracoesCasoMedio.txt", "w");
    FILE *fp_pior_caso = fopen("InsertionSortIteracoesPiorCaso.txt", "w");
    if (fp_caso_medio == NULL || fp_pior_caso == NULL) {
        printf("Erro ao abrir os arquivos.\n");
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
            int iteracoes = 0;
            insertionSort(arr_caso_medio, tam, &iteracoes);
            fprintf(fp_caso_medio, "%d ", iteracoes); // Gravando no arquivo de saída
        }
        fprintf(fp_caso_medio, "\n");

        // Executando 30 repetições para obter o pior caso
        for (int j = 0; j < 30; j++) {
            int iteracoes = 0;
            insertionSort(arr_pior_caso, tam, &iteracoes);
            fprintf(fp_pior_caso, "%d ", iteracoes); // Gravando no arquivo de saída
        }
        fprintf(fp_pior_caso, "\n");

        // Indicando a conclusão do processamento para o tamanho atual
        printf("%d concluida\n", tam);

        // Desalocando a memória dos vetores
        free(arr_caso_medio);
        free(arr_pior_caso);
    }
    // Fechando os arquivos de saída
    fclose(fp_caso_medio);
    fclose(fp_pior_caso);
    return 0;
}
