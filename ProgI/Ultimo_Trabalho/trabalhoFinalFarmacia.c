//Alunos Eduardo Alves Calza e Isabela Estefanuto

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structs
typedef struct {
    int codigo;
    char nome[50];
    float preco;
    int estoque;
    int generico;
    char categoria[50];
    char fabricante[50];
} Registro;

//Cadastrar registro utilizando arquivo binario
void cadastrarRegistro(){
    Registro registro;
    FILE *arquivo;
    arquivo = fopen("arquivo.bin", "ab");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo \n");
    }else{
        printf("Digite o codigo do registro: ");
        scanf("%d", &registro.codigo);
        printf("Digite o nome: ");
        scanf("%s", &registro.nome);
        printf("Digite o preco: ");
        scanf("%f", &registro.preco);
        printf("Digite a quantidade em estoque: ");
        scanf("%d", &registro.estoque);
        printf("Digite o generico(1 para sim e 0 para nao): ");
        scanf("%d", &registro.generico);
        printf("Digite a categoria: ");
        scanf("%s", &registro.categoria);
        printf("Digite o fabricante: ");
        scanf("%s", &registro.fabricante);
        fwrite(&registro, sizeof(Registro), 1, arquivo);
        fclose(arquivo);
        printf("Produto cadastrado com sucesso\n");
    }
}

//Remover registro por codigo
void removerRegistro(int codigo){
    Registro registro;
    FILE *arquivo;
    FILE *arquivoTemp;
    arquivo = fopen("arquivo.bin", "rb");
    arquivoTemp = fopen("arquivoTemp.bin", "wb");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo \n");
    }else{
        while(fread(&registro, sizeof(Registro), 1, arquivo)){
            if(registro.codigo != codigo){
                fwrite(&registro, sizeof(Registro), 1, arquivoTemp);
            }
        }
        fclose(arquivo);
        fclose(arquivoTemp);
        remove("arquivo.bin");
        rename("arquivoTemp.bin", "arquivo.bin");
        printf("Produto Removido com sucesso\n");
    }
}

//Alterar registro por codigo
void alterarRegistro(int codigo){
    Registro registro;
    FILE *arquivo;
    arquivo = fopen("arquivo.bin", "rb+");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo \n");
    }else{
        while(fread(&registro, sizeof(Registro), 1, arquivo)){
            if(registro.codigo == codigo){
                printf("Digite o codigo desse registro: ");
                scanf("%d", &registro.codigo);
                printf("-----NOVO PRODUTO-----\n");
                printf("Digite o nome: ");
                scanf("%s", &registro.nome);
                printf("Digite o preco: ");
                scanf("%f", &registro.preco);
                printf("Digite a quantidade em estoque: ");
                scanf("%d", &registro.estoque);
                printf("Digite o generico(1 para sim e 0 para nao): ");
                scanf("%d", &registro.generico);
                printf("Digite a categoria: ");
                scanf("%s", &registro.categoria);
                printf("Digite o fabricante: ");
                scanf("%s", &registro.fabricante);
                fseek(arquivo, -sizeof(Registro), SEEK_CUR);
                fwrite(&registro, sizeof(Registro), 1, arquivo);
                break;
            }
        }
        fclose(arquivo);
        printf("Produto alterado com sucesso\n");
    }
}


//Consultar registro por codigo
void consultarRegistro(int codigo){
    Registro registro;
    FILE *arquivo;
    arquivo = fopen("arquivo.bin", "rb");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo \n");
    }else{
        while(fread(&registro, sizeof(Registro), 1, arquivo)){
            if(registro.codigo == codigo){
                printf("Codigo: %d \n", registro.codigo);
                printf("Nome: %s \n", registro.nome);
                printf("Preco: %.2f \n", registro.preco);
                printf("Estoque: %d \n", registro.estoque);
                printf("Generico: %d \n", registro.generico);
                printf("Categoria: %s \n", registro.categoria);
                printf("Fabricante: %s \n", registro.fabricante);
                break;
            }
        }
        fclose(arquivo);
    }
}

//Exibir todos os registros
void exibirRegistros(){
    Registro registro;
    FILE *arquivo;
    arquivo = fopen("arquivo.bin", "rb");
    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo \n");
    }else{
        while(fread(&registro, sizeof(Registro), 1, arquivo)){
            printf("Codigo: %d \n", registro.codigo);
            printf("Nome: %s \n", registro.nome);
            printf("Preco: %.2f \n", registro.preco);
            printf("Estoque: %d \n", registro.estoque);
            printf("Generico: %d \n", registro.generico);
            printf("Categoria: %s \n", registro.categoria);
            printf("Fabricante: %s \n", registro.fabricante);
            printf("\n");
        }
        fclose(arquivo);
    }
}

//Menu
void menu(){
    int opcao;
    int codigo;
    do{
        system("cls");
        printf("      ---------------------------------------------------------\n");
        printf("     |                          MENU                           |\n");
        printf("      ---------------------------------------------------------\n\n");

        printf("      ---------------------------------------------------------\n");
        printf("     |[1] Cadastrar \n     |[2] Consultar \n     |[3] Alterar\n     |[4] Remover\n     |[5] Exibir\n     |[6] Sair\n");
        printf("      ---------------------------------------------------------\n");
        scanf("%d", &opcao);
        switch(opcao){
            case 1:
                system("cls");
                cadastrarRegistro();
                system("pause");
                break;
            case 2:
                system("cls");
                printf("Digite o codigo do registro que deseja consultar: ");
                scanf("%d", &codigo);
                consultarRegistro(codigo);
                system("pause");
                break;
            case 3:
                system("cls");
                printf("Digite o codigo do registro que deseja alterar: ");
                scanf("%d", &codigo);
                alterarRegistro(codigo);
                system("pause");
                break;
            case 4:
                system("cls");
                printf("Digite o codigo do registro que deseja remover: ");
                scanf("%d", &codigo);
                removerRegistro(codigo);
                system("pause");
                break;
            case 5:
                system("cls");
                exibirRegistros();
                system("pause");
                break;
            case 6:
                printf("Saindo... \n");
                break;
            default:
                printf("Opcao invalida \n");
                system("pause");
                break;
        }
    }while(opcao != 6);
}

int main(){
    menu();
    return 0;
}