#include "estoque.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 30
#define NOME 20

// Struct tipo Medicamento
typedef struct medicamento{
    char nome[NOME];
    int codigo;
    float valor;
    int data[3];
} Medicamento;

// Struct arvore
typedef struct arvore{
    struct medicamento *m;
    struct arvore *esquerda;
    struct arvore *direita;
} Arvore;

// Função para ler o arquivo e chamar as propriedades do sistema
void LerArquivo(){

    Arvore *a = NULL;
    Medicamento *med = NULL, medicamentoAux;
    char infoDoArquivo[M];

    FILE *fp_entrada = fopen("entrada.txt", "r");
    if(fp_entrada == NULL){ 
        printf("Erro de abertura.\n");
        exit(1);
    } // Verifica se o arquivo foi aberto corretamente 

    FILE *fp_saida = fopen("saida.txt", "w");
    if(fp_saida == NULL){ 
        printf("Erro de abertura.\n");
        exit(1);
    } // Verifica se o arquivo foi aberto corretamente 

    while(fscanf(fp_entrada, "%s", infoDoArquivo) != EOF){
        // Ao ler "MEDICAMENTO" chama a funcao de criar medicamento
        if(strcmp(infoDoArquivo, "MEDICAMENTO") == 0){
            fscanf(fp_entrada, "%s %d %f %d %d %d", medicamentoAux.nome, &medicamentoAux.codigo, &medicamentoAux.valor, &medicamentoAux.data[0], &medicamentoAux.data[1], &medicamentoAux.data[2]);
            // Le e armazena os dados numa struct Medicamento
            med = CriaMedicamento(medicamentoAux.nome, medicamentoAux.codigo, medicamentoAux.valor, medicamentoAux.data);
            a = InsereArvoreMedicamento(fp_saida, a, med); // Insere um novo noh na arvore
        }
        // Ao ler "RETIRA" chama a funcao de retirar um medicamento da arvore
        else if(strcmp(infoDoArquivo, "RETIRA") == 0){
            fscanf(fp_entrada, "%d", &medicamentoAux.codigo);
            a = RetiraArvoreMedicamento(fp_saida, a, medicamentoAux.codigo);
        }
        // Ao ler "IMPRIME_ARVORE" (imprime os medicamentos em ordem de codigo)
        else if(strcmp(infoDoArquivo, "IMPRIME_ARVORE") == 0){
            ImprimeArvoreMedicamentos(fp_saida, a);
        }
        // Ao ler "ATUALIZA_PRECO" chama a funcao que atualiza o preco do medicamento informado
        else if(strcmp(infoDoArquivo, "ATUALIZA_PRECO") == 0){
            fscanf(fp_entrada, "%d %f", &medicamentoAux.codigo, &medicamentoAux.valor);
            AtualizaPreco(fp_saida, a, medicamentoAux.codigo, medicamentoAux.valor);
        }
        // Ao ler "VERIFICA_VALIDADE" chama a funcao que verifica a validade dos medicamentos
        else if(strcmp(infoDoArquivo,"VERIFICA_VALIDADE") == 0){
            fscanf(fp_entrada, "%d %d %d", &medicamentoAux.data[0], &medicamentoAux.data[1], &medicamentoAux.data[2]);
            if(VerificaArvoreValidade(fp_saida, a, medicamentoAux.data)){
                fprintf(fp_saida, "SEM MEDICAMENTO VENCIDO");
            }
        }
        // Ao ler "VERIFICA_ARVORE" chama a funcao que verifica se o medicamento esta na árvore
        else if(strcmp(infoDoArquivo, "VERIFICA_ARVORE") == 0){
            fscanf(fp_entrada, "%d", &medicamentoAux.codigo);
            VerificaArvoreMedicamento(fp_saida, a, medicamentoAux.codigo);
        }
        // Ao ler "FIM" libera a memoria e encerra o programa
        else if(strcmp(infoDoArquivo, "FIM") == 0){
            LiberaMemoria(a); // Funcao para liberar a memoria da árvore
            exit(0);
        }
    }
    fclose(fp_entrada);
    fclose(fp_saida);
}

Arvore *CriaArvore(){
    return NULL;
}

// Armazena os dados num noh da struct Medicamento
Medicamento *CriaMedicamento(char *nome, int codigo, float valor, int *data_de_validade){
    Medicamento *novo = (Medicamento*)malloc(sizeof(Medicamento));
    if(novo == NULL){ // Cria um noh alocado dinamicamente e verifica se esta correto
        printf("Erro de alocacao.\n");
        exit(1);
    }

    // Adiciona dado por dado
    strncpy(novo->nome, nome, M-1);
    novo->nome[M-1] = '\0';
    novo->codigo = codigo;
    novo->valor = valor;
    for(int i=0; i<3; i++){
        novo->data[i] = data_de_validade[i];
    }
    return novo; // Retorna o novo noh
}

// Insere de forma ordenada os nohs Medicamentos
Arvore *InsereArvoreMedicamento(FILE* fp, Arvore *a, Medicamento *m){
    if(a == NULL){
        a = (Arvore*)malloc(sizeof(Arvore)); // Aloca memoria para um novo noh
        a->m = m;
        a->esquerda = a->direita = NULL; // Inicializa esquerda e direita como NULL

        fprintf(fp, "MEDICAMENTO %s %d ADICIONADO\n", m->nome, m->codigo);
    }
    else if(m->codigo < a->m->codigo){ // Se for menor, vai ser salvo na esquerda
        a->esquerda = InsereArvoreMedicamento(fp, a->esquerda, m);
    }
    else if(m->codigo > a->m->codigo){ // Se for maior, vai ser salvo na direita
        a->direita = InsereArvoreMedicamento(fp, a->direita, m);
    }
    return a; // Retorna o ponteiro para a raiz a
}

// Funcao para imprimir os medicamentos em ordem por codigo
void ImprimeArvoreMedicamentos(FILE *fp, Arvore *a){
    if (a == NULL) { // Verifica se a arvore esta vazia
        return;
    }
    // Percorre a subarvore esquerda
    ImprimeArvoreMedicamentos(fp, a->esquerda);
    // Imprime o noh atual
    fprintf(fp, "%s %d %.1f %d %d %d\n", a->m->nome, a->m->codigo, a->m->valor, a->m->data[0], a->m->data[1], a->m->data[2]);
    // Percorre a subarvore direita
    ImprimeArvoreMedicamentos(fp, a->direita);
}

// Funcao para atualizar o preço de um medicamento na árvore
void AtualizaPreco(FILE *fp, Arvore *a, int codigoMedicamentoAlterPreco, float valorPrecoAlterado) {
    if (a == NULL) { // Verifica se esta vazia 
        fprintf(fp, "MEDICAMENTO ATUALIZAR PREÇO NÃO ENCONTRADO\n");
        return;
    }

    // Procura o noh com o código especificado
    if (codigoMedicamentoAlterPreco < a->m->codigo) {
        AtualizaPreco(fp, a->esquerda, codigoMedicamentoAlterPreco, valorPrecoAlterado);
    } else if (codigoMedicamentoAlterPreco > a->m->codigo) {
        AtualizaPreco(fp, a->direita, codigoMedicamentoAlterPreco, valorPrecoAlterado);
    } else {
        // Medicamento encontrado, atualiza o preco
        a->m->valor = valorPrecoAlterado;
        fprintf(fp, "PREÇO ATUALIZADO %s %d %.1f\n", a->m->nome, a->m->codigo, a->m->valor);
    }
}

/* Funcao que verifica se ha o medicamento com o codigo informado dentro da arvore*/
void VerificaArvoreMedicamento(FILE *fp, Arvore *a, int id_medicamento) {
    if (a == NULL) { // Verifica se esta vazia 
        fprintf(fp, "MEDICAMENTO NAO ENCONTRADO NA ARVORE\n");
        return;
    }

    // Verifica se o codigo do medicamento atual eh o procurado
    if (a->m->codigo == id_medicamento) {
        fprintf(fp, "MEDICAMENTO ENCONTRADO %s %d %.1f %d %d %d\n", 
            a->m->nome, a->m->codigo, a->m->valor, a->m->data[0], a->m->data[1], a->m->data[2]);
    } 
    else if (id_medicamento < a->m->codigo) {
        // Se o codigo procurado eh menor, percorre a subarvore esquerda
        VerificaArvoreMedicamento(fp, a->esquerda, id_medicamento);
    } 
    else {
        // Se o codigo procurado eh maior, percorre a subarvore direita
        VerificaArvoreMedicamento(fp, a->direita, id_medicamento);
    }
}


// Funcao para verificar a validade dos medicamentos em uma arvore
int VerificaArvoreValidade(FILE *saida, Arvore *a, int *data) {
    int count = 0;
    if (a == NULL) {
        return 0;
    }
    if ((a->m->data[2] < data[2]) ||  // Verifica ano
        (a->m->data[2] == data[2] && a->m->data[1] < data[1]) || // Caso ano == ano verifica mes
        (a->m->data[2] == data[2] && a->m->data[1] == data[1] && a->m->data[0] < data[0])) { // caso ano == ano, mes == mes, verifica os dias
            count = 1; // Contador para verificar se teve ou nao medicamentos vencidos
        } 

    if(count) { // Printa o medicamento vencido
        fprintf(saida, "MEDICAMENTO %s %d VENCIDO\n", a->m->nome, a->m->codigo);
    }
    
    // Recursividade para continuar procurando na esquerda e na direita
    VerificaArvoreValidade(saida, a->esquerda, data);
    VerificaArvoreValidade(saida, a->direita, data);

    return 0;
}

// Função que retira o medicamento da arvore
Arvore *RetiraArvoreMedicamento(FILE *saida, Arvore *a, int id_medicamento){
    if(a == NULL){ // Arvore vazia, ou nao encontra o valor 
        return NULL;
    }
    if(a->m->codigo > id_medicamento){ // Caso id < m->codigo entao procura na esquerda
        a->esquerda = RetiraArvoreMedicamento(saida, a->esquerda, id_medicamento);
    }
    else if(a->m->codigo < id_medicamento){ // Caso id > m->codigo entao procura na direita
        a->direita = RetiraArvoreMedicamento(saida, a->direita, id_medicamento);
    }
    //Achou o noh a ser removido
    else{
        // Caso 1: o noh atual nao possui filhos algum
        if(a->esquerda == NULL && a->direita == NULL){
            fprintf(saida,"MEDICAMENTO %s %d RETIRADO\n", a->m->nome, a->m->codigo);
            free(a);
            a = NULL;
        }
        // Caso 2: o noh atual nao possui filho a esquerda
        else if(a->esquerda == NULL){
            Arvore *aux = a;
            a = a->direita;// O noh atual comeca apontar para o da direita
            fprintf(saida,"MEDICAMENTO %s %d RETIRADO\n", aux->m->nome, aux->m->codigo);
            free(aux->m);
            free(aux);
        }
        // Caso 3: o noh atual nao possui filho a direita
        else if(a->direita == NULL){
            Arvore *aux = a;
            a = a->esquerda;// Noh atual começa apontar para o da esquerda
            fprintf(saida,"MEDICAMENTO %s %d RETIRADO\n", aux->m->nome, aux->m->codigo);
            free(aux->m);
            free(aux);
        }
        // Caso 4: o noh atual possui filhos a direita e esquerda
        else{
            Arvore *aux2 = a->esquerda;
            while(aux2->direita != NULL){ // Procura o maior valor a direita da subarvore da esquerda
                aux2 = aux2->direita;
            }
            Medicamento *temp = a->m;
            a->m = aux2->m;
            aux2->m = temp;
            a->esquerda = RetiraArvoreMedicamento(saida, a->esquerda, id_medicamento);
        }
    }
    return a;
}

// Libera a memoria da arvore e dos medicamentos
void LiberaMemoria(Arvore *a) {
    if (a == NULL) {
        return;  // Base da recursao, quando a arvore esta vazia
    }
    // Libera os nos das subarvores esquerda e direita
    LiberaMemoria(a->esquerda);
    LiberaMemoria(a->direita);
    // Libera o medicamento armazenado no noh
    free(a->m);
    // Libera o proprio noh da árvore
    free(a);
}
