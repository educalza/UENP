/*Fa ̧ca um programa para montar um banco de dados relacionado a um jogo, o qual deve
conter informa ̧c ̃oes de in ́umeros monstros e personagens. Inicialmente, o programa deve
receber a quantidade de personagens/monstros a ser cadastrada. Os dados a serem colocados
no banco s ̃ao:
• Uma sequencia de caracteres, que poder ́a ter no m ́aximo 20 caracteres, representando
um nome.
• 5 inteiros, representando ID, Level, Vida, Ataque e Defesa.
Ao final, o programa deve exibir o banco de dados cadastrado.*/
#include <stdio.h>

struct MonstrosEPersonagens{
    char nome[20];
    int ID;
    int level;
    int vida;
    int ataque;
    int defesa;
};

void cadastroPersonagens(struct MonstrosEPersonagens Personagens[], int numeroPersonagens){

    printf("----Cadastro de personagens----\n\n");
    for(int i=0; i<numeroPersonagens; i++){
        printf("Nome: ");
        setbuf(stdin, NULL);
        fgets(Personagens[i].nome, 20, stdin);
        printf("ID: ");
        scanf("%d", &Personagens[i].ID);
        printf("Level: ");
        scanf("%d", &Personagens[i].level);
        printf("Vida: ");
        scanf("%d", &Personagens[i].vida);
        printf("Ataque: ");
        scanf("%d", &Personagens[i].ataque);
        printf("Defesa: ");
        scanf("%d", &Personagens[i].defesa);
        printf("\n\n");
        }
    }
void PrintarPersonagens(struct MonstrosEPersonagens Personagens[], int numeroPersonagens){

    printf("Ficha dos personagens. \n\n");
    for(int i=0; i<numeroPersonagens; i++){
        printf("Personagem %d\n", i+1);
        printf("Nome: %s", Personagens[i].nome);
        printf("ID: %d\n", Personagens[i].ID);
        printf("Level: %d\n", Personagens[i].level);
        printf("Vida: %d\n", Personagens[i].vida);
        printf("Ataque: %d\n", Personagens[i].ataque);
        printf("Defesa: %d\n", Personagens[i].defesa);
        printf("\n\n");
    }
}

void cadastrarMonstros(struct MonstrosEPersonagens Monstros[], int numeroMonstro){

    printf("----Cadastro de monstro----\n\n");
        for(int i=0; i<numeroMonstro; i++){
            printf("Nome: ");
            setbuf(stdin, NULL);
            fgets(Monstros[i].nome, 20, stdin);
            printf("ID: ");
            scanf("%d", &Monstros[i].ID);
            printf("Level: ");
            scanf("%d", &Monstros[i].level);
            printf("Vida: ");
            scanf("%d", &Monstros[i].vida);
            printf("Ataque: ");
            scanf("%d", &Monstros[i].ataque);
            printf("Defesa: ");
            scanf("%d", &Monstros[i].defesa);
            printf("\n\n");
        }
}
void printarMonstros(struct MonstrosEPersonagens Monstros[], int numeroMonstro){

    printf("Ficha dos monstros. \n\n");
    for(int i=0; i<numeroMonstro; i++){
        printf("Monstro %d\n", i+1);
        printf("Nome: %s", Monstros[i].nome);
        printf("ID: %d\n", Monstros[i].ID);
        printf("Level: %d\n", Monstros[i].level);
        printf("Vida: %d\n", Monstros[i].vida);
        printf("Ataque: %d\n", Monstros[i].ataque);
        printf("Defesa: %d\n", Monstros[i].defesa);
        printf("\n\n");
    }
}

int main(){

    int mon=0, per=0;

    printf("Digite quantos mosntros serao cadastrados: ");
    scanf("%d", &mon);
    printf("Digite quantos personagens serao cadastrados: ");
    scanf("%d", &per);

    struct MonstrosEPersonagens monstros[mon];
    struct MonstrosEPersonagens personagens[per];

    char analise;

    printf("Qual sera cadastrado primeiro, personagem ou monstros?(p - personagem, m - monstro): ");
    scanf(" %c", &analise);

    if(analise=='p'){
        cadastroPersonagens(personagens, per);
        cadastrarMonstros(monstros, mon);
    }
    else if(analise=='m'){
        cadastrarMonstros(monstros, mon);
        cadastroPersonagens(personagens, per);
    }

    if(analise=='p'){
        PrintarPersonagens(personagens, per);
        printarMonstros(monstros, mon);
    }
    else if(analise=='m'){
        printarMonstros(monstros, mon);
        PrintarPersonagens(personagens, per);
    }

    return 0;
}

