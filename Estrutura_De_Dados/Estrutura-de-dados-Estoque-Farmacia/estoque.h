#ifndef ESTOQUE_H_INCLUDED
#define ESTOQUE_H_INCLUDED

#include <stdio.h>

// Tipo que define o Medicamento
typedef struct medicamento Medicamento;

// Tipo que define a Lista
typedef struct lista Lista;

// Função que lê o arquivo e chama cada função
void LerArquivo();

// Função que cria a lista inicializando com NULL
Lista *CriaLista();

// Funçao que cria o medicamento e armazena num nó de lista
Medicamento *CriaMedicamento(char *nome, int codigo, float valor, int *data_de_validade);

// Função que adiciona o nó medicamento numa lista
Lista *InsereListaMedicamento(FILE *fp, Lista *l, Medicamento *m);

// Função que retira um medicamento da lista
Lista *RetiraListaMedicamento(FILE *fp, Lista *l, int id_medicamento);

// Função que imprime todos os medicamentos da lista
void ImprimeListaMedicamentos(FILE *fp, Lista *l);

// Função que atualiza o preço de um medicamento
void AtualizaPreco(FILE *fp, Lista *l, int codigoMedicamentoAlterPreco, float valorPrecoAlterado);

// Função que verifica a validade dos medicamentos
void VerificaListaValidade(FILE *fp, Lista *l, int *data);

// Função que verifica se há o medicamento na lista
void VerificaListaMedicamento(FILE *fp, Lista *l, int id_medicamento);

// Ordena a lista de medicamentos por valor
Lista *OrdenaListaValor(Lista *l);

// Ordena a lista de medicamentos por validade 
Lista *OrdenaListaVencimento(Lista *l);

// Libera a memória da alocação struct Medicamento e da lista lst
void LiberaMemoria(Lista *l);

#endif // ESTOQUE_H_INCLUDED