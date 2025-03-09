#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* Definição de uma estrutura de nó (Lista) contendo um inteiro (data) e
um ponteiro para o próximo nó. */

struct lista{
    int data;
    struct lista *prox;
}; typedef struct lista Lista;

Lista *cria_lst(void){
    return NULL;
}

/* Criação de uma função (cria_noLista) para alocar e inicializar novos
nós. */

Lista *cria_noLista(Lista *lst, int valor){
    Lista *novo = (Lista*)malloc(sizeof(Lista));
    novo->data = valor;
    novo->prox = lst;
    return novo;
}

/* Implementação de uma função (insere_ordenado) para inserir valores
na lista mantendo a ordem decrescente. */

Lista *insere_ordenado(Lista *lst, int valor){
    Lista *novo;
    Lista *ant = NULL;
    Lista * p = lst;

    while(p!=NULL&& p->data > valor){
        ant = p;
        p = p->prox;
    }
    novo = (Lista*)malloc(sizeof(Lista));
    novo->data = valor;
    if(ant == NULL){
        novo->prox = lst;
        lst = novo;
    }else{
        novo->prox = ant->prox;
        ant->prox = novo;
    }
    return lst;
}

void imprime_lst(Lista *lst){
    Lista *p;
    for(p=lst; p!=NULL; p=p->prox){
        printf("%d ", p->data);
    }
}

/* Função (free_lista) para liberar a memória alocada pela lista após cada
medição. */

void free_lista(Lista *lst){
    Lista *p = lst;
    Lista *t;
    while(p!=NULL){
        t = p->prox;
        free(p);
        p = t;
    }
}

int main(){

    clock_t start_t, end_t;
    double total_t;

    Lista *lst;
    lst = cria_lst();

/*     printf("Digite os numeros.\n");
    for(int i=0; i<10; i++){
        int valor=0;
        scanf("%d", &valor);
        lst = insere_ordenado(lst, valor);
    }
    printf("\nLista encerrada.\n");
    imprime_lst(lst); */   
    FILE *arquivo;
    arquivo = fopen("tempo.txt", "w+");
    int num_vezes = 0;
    for(int k=0; k<10; k++){
        num_vezes += 5000;
        for (int i=0;i<30;i++){
            start_t = clock();
            for(int j=num_vezes; j>0; j--){
                insere_ordenado(lst, i);
            }
        }
            end_t = clock();
            double total_t = (double)(end_t-start_t);
            fprintf(arquivo, "Linha[%d]: %f\n", num_vezes, total_t);
    }

    fclose(arquivo);
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
    printf("\nTotal time: %f\n", total_t);

    free(lst);
    return 0;

}