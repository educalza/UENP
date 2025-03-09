/*Defina uma estrutura adequada para armazenar os dados de um livro, contendo: c ́odigo,
t ́ıtulo, autor e assunto. Com base nessa estrutura, fa ̧ca o cadastro de livros usando a
aloca ̧c ̃ao dinˆamica de mem ́oria e depois mostre esses dados na tela. Por fim, implemente 
uma fun ̧cao que possibilite a busca de um determinado livro por meio de seu c ́odigo. A
busca deve retorna 1 caso o livro seja encontrado e 0, caso contr ́ario.*/

#include <stdio.h>
#include <stdlib.h>

struct dadosLivro{
    int codigo;
    char titulo[30];
    char autor[30];
    char assunto[100];
};

int BuscaLivro(struct dadosLivro *livro, int n);

int main(){

    struct dadosLivro *livro;
    int n=0;

    printf("Digite quantos livros ira armazenar: ");
    scanf("%d", &n);
    livro = (struct dadosLivro*)calloc(n, sizeof(struct dadosLivro));
    
    for(int i=0; i<n; i++){
        printf("Codigo: ");
        scanf("%d", &livro[i].codigo);
        printf("Titulo: ");
        setbuf(stdin, NULL);
        fgets(livro[i].titulo, 30, stdin);
        printf("Autor: ");
        setbuf(stdin, NULL);
        fgets(livro[i].autor, 30, stdin);
        printf("Assunto: ");
        setbuf(stdin, NULL);
        fgets(livro[i].assunto, 100, stdin);
        printf("\n");
    }
    printf("\nLivros registrados:\n");
    for(int i=0; i<n; i++){
        printf("Codigo: %d\n", livro[i].codigo);
        printf("Titulo: %s", livro[i].titulo);
        printf("Autor: %s", livro[i].autor);
        printf("Assunto: %s", livro[i].assunto);
        printf("\n");
    }
    printf("\n");

    int funcao = BuscaLivro(livro, n);

    if(funcao == 1){
        printf("O livro esta disponivel.\n");
    }
    else
        printf("O livro nao esta disponivel");

    for(int i=0; i<n; i++){
        free(livro);
    }
    return 0;
}

int BuscaLivro(struct dadosLivro *livro, int n){

    int codigoVerificacao;

    printf("Digite um codigo para verificar se tem o livro.\n");
    scanf("%d", &codigoVerificacao);
    for(int i=0; i<n; i++){
        if(livro[i].codigo==codigoVerificacao){
            return 1;
        }
    }
    return 0;
}