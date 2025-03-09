#include "estoque.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define M 30

// Tipo que define o medicamento
struct medicamento{
    char nome[20];
    int codigo;
    float valor;
    int data[3];
}; typedef struct medicamento Medicamento;

// Tipo que define a lista
struct lista{
    Medicamento *m;
    struct lista *prox;
}; typedef struct lista Lista;


void LerArquivo(){

    Lista *lst = NULL;
    Medicamento *med = NULL, medicamentoAux;
    char infoDoArquivo[M];

    FILE *fp_entrada = fopen("entrada.txt", "r");
    if(fp_entrada == NULL){ 
        printf("Erro de abertura.\n"); 
        exit(1);
    } // verifica se o arquivo foi aberto corretamente 

    FILE *fp_saida = fopen("saida.txt", "w");
    if(fp_saida == NULL){ 
        printf("Erro de abertura.\n");
        exit(1);
    } // verifica se o arquivo está vazio

    while(fscanf(fp_entrada, "%s", infoDoArquivo) != EOF){
        // realizar cada verificação de acordo com o que está escrito no arquivo de entrada, usando strcmp
        // Ao ler "MEDICAMENTO" chama a função de criar medicamento
        if(strcmp(infoDoArquivo, "MEDICAMENTO") == 0){
            fscanf(fp_entrada, "%s %d %f %d %d %d", medicamentoAux.nome, &medicamentoAux.codigo, &medicamentoAux.valor, &medicamentoAux.data[0], &medicamentoAux.data[1], &medicamentoAux.data[2]);
            // le e armazena os  dados numa estruct Medicamento
            med = CriaMedicamento(medicamentoAux.nome, medicamentoAux.codigo, medicamentoAux.valor, medicamentoAux.data); // insere os dados no ponteiro de struct "med"
            lst = InsereListaMedicamento(fp_saida, lst, med); // insere uum novo nó na lista 
        }

        // Ao ler "RETIRA" chama a função de retirar um medicamento da lista
        else if(strcmp(infoDoArquivo, "RETIRA") == 0){
            fscanf(fp_entrada, "%d", &medicamentoAux.codigo);
            lst = RetiraListaMedicamento(fp_saida, lst, medicamentoAux.codigo);
        }

        // Ao ler "IMPRIME_LISTA" chama a função de imprimir os medicamentos que há na lista
        else if(strcmp(infoDoArquivo, "IMPRIME_LISTA") == 0){
            ImprimeListaMedicamentos(fp_saida, lst);
        }

        // Ao ler "ATUALIZA_PRECO" chama a função que atualiza o preço do medicamento informado
        else if(strcmp(infoDoArquivo, "ATUALIZA_PRECO") == 0){
            fscanf(fp_entrada, "%d %f", &medicamentoAux.codigo, &medicamentoAux.valor);
            AtualizaPreco(fp_saida, lst, medicamentoAux.codigo, medicamentoAux.valor);
        }

        // Ao ler "VERIFICA_VALIDADE" chama a função que verifica a validade dos medicamentos
        else if(strcmp(infoDoArquivo,"VERIFICA_VALIDADE") == 0){
            fscanf(fp_entrada, "%d %d %d", &medicamentoAux.data[0], &medicamentoAux.data[1], &medicamentoAux.data[2]);
            VerificaListaValidade(fp_saida, lst, medicamentoAux.data);
        }

        // Ao ler "VERIFICA_LISTA" chama a função que verifica se o medicamento está na lista
        else if(strcmp(infoDoArquivo, "VERIFICA_LISTA") == 0){
            fscanf(fp_entrada, "%d", &medicamentoAux.codigo);
            VerificaListaMedicamento(fp_saida, lst, medicamentoAux.codigo);
        }

        // Ao ler "ORDENA_LISTA_VALIDADE" chama a função que ordena a lista de medicamentos por validade
        else if(strcmp(infoDoArquivo, "ORDENA_LISTA_VALIDADE") == 0){
            lst = OrdenaListaVencimento(lst);
        }

        // Ao ler "ORDENA_LISTA_VALOR" chama a função que ordena a lista de medicamentos por valor
        else if(strcmp(infoDoArquivo, "ORDENA_LISTA_VALOR") == 0){
            lst = OrdenaListaValor(lst);
        }

        // Ao ler "FIM" libera a memória e encerra o programa
        else if(strcmp(infoDoArquivo, "FIM") == 0){
            LiberaMemoria(lst);
            exit(0);
        }
    }
    fclose(fp_entrada);
    fclose(fp_saida);
}


//Lista * CriaLista () ;
Lista *CriaLista(){
    return NULL;
}


/* Cria um novo medicamento */
//Medicamento * CriaMedicamento ( FILE * fp , char * nome ,int codigo , float valor ,int * data_de_validade );
Medicamento *CriaMedicamento(char *nome, int codigo, float valor, int *data_de_validade){

    Medicamento *novo = (Medicamento*)malloc(sizeof(Medicamento));
    if(novo == NULL){ 
        printf("Erro de abertura.\n"); 
        exit(1); 
    } // verifica se "novo" foi inicializado corretamente

    // Adiciona os valores no novo nó
    strncpy(novo->nome, nome, M - 1);
    novo->nome[M - 1] = '\0';
    
    novo->codigo = codigo;
    novo->valor = valor;
    for(int i=0; i<3; i++){
        novo->data[i] = data_de_validade[i];
    }
    return novo;
}


/* Insere um medicamento em uma lista */
//Lista * InsereListaMedicamento ( FILE * fp , Lista * l , Medicamento * m );
Lista *InsereListaMedicamento(FILE *fp, Lista *l, Medicamento *m){

    // Insere o medicamento na lista
    Lista *novo = (Lista*)malloc(sizeof(Lista));
    if(novo == NULL){
        printf("Erro de alocacao.\n");
        exit(1);
    }
    
    novo->m = m;
    novo->prox = l;
    fprintf(fp, "MEDICAMENTO %s %d ADICIONADO\n", m->nome, m->codigo);
    return novo;
}


/* Retira um medicamento de uma determinada lista */
//Lista * RetiraListaMedicamento ( FILE * fp , Lista * l , int id_medicamento );
Lista *RetiraListaMedicamento(FILE *fp, Lista *l, int id_medicamento){

    Lista *ant = NULL, *p = l;

    while(p!=NULL && p->m->codigo != id_medicamento){
        ant = p;
        p = p->prox;
    }
    // Caso p == NULL o medicamento não foi encontrado
    if(p == NULL){ 
        fprintf(fp, "MEDICAMENTO RETIRA NAO ENCONTRADO\n");
        return l;
    }
    // O primeiro numero é o escolhido
    if(ant == NULL){
        l = p->prox;
    }
    // Retira o nó
    else{
        ant->prox = p->prox;
    }

    // Printa o medicamento retirado no arquivo de saída
    fprintf(fp, "MEDICAMENTO %s %d RETIRADO\n", l->m->nome, id_medicamento);
    free(p->m);
    free(p);

    return l;
}


/* Imprime todos os medicamentos de uma lista */
//void ImprimeListaMedicamentos ( FILE * fp , Lista * l) ;
void ImprimeListaMedicamentos(FILE *fp, Lista *l){

    Lista *p;
    for(p = l; p!=NULL; p=p->prox){
        fprintf(fp, "%s %d %.1f %d %d %d\n", p->m->nome, p->m->codigo, p->m->valor, p->m->data[0], p->m->data[1], p->m->data[2]);
    }
}


void AtualizaPreco(FILE *fp, Lista *l, int codigoMedicamentoAlterPreco, float valorPrecoAlterado){

    Lista *p = l;
    int encontrou = 0; // Confere se encontrou o medicamento pedido
    while(p!=NULL){
        if(p->m->codigo == codigoMedicamentoAlterPreco){
            p->m->valor = valorPrecoAlterado;
            // Atualiza o preço do medicamento e informa no arquivo
            fprintf(fp, "PRECO ATUALIZADO %s %d %.1f\n", p->m->nome, p->m->codigo, p->m->valor);
            encontrou = 1;
            break;
        }
        p = p->prox;
    }
    // Caso não encontrou o medicamento informado 
    if(!encontrou){
        fprintf(fp, "MEDICAMENTO ATUALIZAR PRECO NAO ENCRONTRADO\n");
    }
}


/* Verifica se existe um medicamento vencido em uma determinada lista */
//int VerificaListaValidade ( FILE * fp , Lista * p , int * data ) ;
void VerificaListaValidade(FILE *fp, Lista *l, int *data){

    int produtoVencido = 0; // Confere se o medicamento está na validade
    for(Lista *p = l; p!=NULL; p=p->prox){
        if ((p->m->data[2] < data[2]) || 
        (p->m->data[2] == data[2] && p->m->data[1] < data[1]) || 
        (p->m->data[2] == data[2] && p->m->data[1] == data[1] && p->m->data[0] < data[0])) {
            produtoVencido = 1;
            fprintf(fp, "MEDICAMENTO %s %d VENCIDO\n", p->m->nome, p->m->codigo);
        }
    }

    if(!produtoVencido){ // Caso não tenha produto vencido, informa que não há medicamento vencido
        fprintf(fp, "MEDICAMENTO VENCIDO NAO ENCONTRADO NA LISTA\n");
    }
}


/* Verifica se um medicamento e s t presente em uma determinada lista */
//int VerificaListaMedicamento ( FILE * fp , Lista * p , int id_medicamento ) ;
void VerificaListaMedicamento(FILE *fp, Lista *l, int id_medicamento){

    Lista *p = l;
    int encontrou = 0; // Verifica se o medicamento está na lista
    while(p!=NULL){
        if(p->m->codigo == id_medicamento){ // Verifica e imprime no arquivo o medicamento encontrado
            fprintf(fp, "MEDICAMENTO ENCONTRADO %s %d %.1f %d %d %d\n", p->m->nome, p->m->codigo, p->m->valor, p->m->data[0], p->m->data[1], p->m->data[2]);
            encontrou = 1;
            break;
        }
        p = p->prox;
    }
    if(encontrou == 0){ // Caso o medicamento não encontrado, imprime que não foi encontrado dentro do arquivo
        fprintf(fp, "MEDICAMENTO NAO ENCONTRADO NA LISTA\n");
    } 
}


/* Ordena Lista pelo valor do medicamento */
Lista *OrdenaListaValor(Lista *p){
    for(Lista *i = p; i != NULL; i = i->prox){
        for(Lista *j = p; j != NULL; j = j->prox){
            if(i->m->valor < j->m->valor){
                Medicamento *aux = i->m;
                i->m = j->m;
                j->m = aux;
            }
        }
    }
    return p;
}

/* Ordena Lista pela data de vencimento do medicamento */
Lista *OrdenaListaVencimento(Lista *p){
    for(Lista *i = p; i != NULL; i = i->prox){
        for(Lista *j = p; j != NULL; j = j -> prox){
            if(i->m->data[2] < j->m->data[2]){
                Medicamento *aux = i->m;
                i->m = j->m;
                j->m = aux;
        }   
            else if(i->m->data[2] == j->m->data[2] && i->m->data[1] < j->m->data[1]){
                Medicamento *aux = i->m;
                i->m = j->m;
                j->m = aux;
        }
            else if(i->m->data[2] == j->m->data[2] && i->m->data[1] == j->m->data[1] && i->m->data[0] < j->m->data[0]){
                Medicamento *aux = i->m;
                i->m = j->m;
                j->m = aux;
            }
        }
    }
    return p;
}

// Libera a memória
void LiberaMemoria(Lista *l){

    Lista *p = l, *proxm = NULL;
    while(p!=NULL){
        proxm = p->prox;
        free(p->m);
        free(p);
        p = proxm;
    }
}