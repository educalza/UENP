#ifndef ESTOQUE_H_INCLUDED
#define ESTOQUE_H_INCLUDED

#include <stdio.h>

// Tipo que define o medicamento
typedef struct medicamento Medicamento;

// Tipo que define a rvore
typedef struct arvore Arvore;

// Função que lê o arquivo e chama cada função
void LerArquivo();

/* Cria uma arvore vazia */
Arvore *CriaArvore();

/* Cria um novo medicamento */
Medicamento *CriaMedicamento(char *nome , int codigo , float valor ,int *data_de_validade) ;

/* Cria um novo nó para a arvore */
Arvore* NovoNo(Medicamento *m);

/* Insere um medicamento pelo codigo na arvore */
Arvore *InsereArvoreMedicamento(FILE *fp, Arvore *p, Medicamento *m);

/* Auxilia encontrar o sucessor (o menor valor à direita)*/
Arvore *Minimo(Arvore *p);

/* Retira um medicamento de uma determinada rvore */
Arvore *RetiraArvoreMedicamento(FILE *fp, Arvore *a, int id_medicamento);

/* Imprime em ordem pelo codigo todos os medicamentos de uma arvore(em ordem )*/
void ImprimeArvoreMedicamentos(FILE *fp, Arvore *a);

/* Atualiza o preco do medicamento indicado pelo codigo*/
void AtualizaPreco(FILE *fp, Arvore *a, int codigoMedicamentoAlterPreco, float valorPrecoAlterado);

/* Verifica se um medicamento está presente em uma determinadaarvore */
void VerificaArvoreMedicamento(FILE *fp, Arvore *a, int id_medicamento);

/* Verifica se existe um medicamento vencido em uma determinada arvore */
int VerificaArvoreValidade(FILE *saida, Arvore *a, int *data);

/* Libera a memória alocada para a arvore*/
void LiberaMemoria(Arvore *a);

#endif // ESTOQUE_H_INCLUDED