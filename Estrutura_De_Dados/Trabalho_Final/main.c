#include <stdio.h>
#include "header.h"

int main()
{
    Set set1, set2, result;
    initSet(&set1);
    initSet(&set2);
    initSet(&result);

    // Testando `add`
    add(&set1, 1);
    add(&set1, 2);
    add(&set1, 3);
    printf("Set A apos add: ");
    print(&set1);
    printf("\n");

    add(&set2, 1);
    add(&set2, 2);
    add(&set2, 3);
    add(&set2, 4);
    printf("Set B apos add: ");
    print(&set2);
    printf("\n");

    // Testando `isIn` e `isNotIn`
    printf("2 esta no Set A? %s\n", isIn(&set1, 2) ? "Sim" : "Nao");
    printf("4 nao esta no Set 1? %s\n", isNotIn(&set1, 4) ? "Sim" : "Nao");
    printf("\n");

    // Testando `removeSet`
    removeSet(&set1, 2);
    printf("Set A apos remover 2: ");
    print(&set1);
    printf("\n");
    printf("Adicao do numero 2 novamente ao set A.\n\n");
    add(&set1, 2);

    // Testando `len`
    printf("Tamanho do Set A: %d\n", len(&set1));
    printf("\n");

    // Testando `unionSet`
    result = unionSet(&set1, &set2);
    printf("Uniao de Set A e Set B: ");
    print(&result);
    printf("\n");
    clear(&result);

    // Testando `intersection`
    result = intersection(&set1, &set2);
    printf("Interseccao de Set A e Set B: ");
    print(&result);
    printf("\n");
    clear(&result);

    // Testando `difference`
    result = difference(&set1, &set2);
    printf("Diferenca (Set A - Set B): ");
    print(&result);
    printf("\n");
    clear(&result);

    // Testando `symmetric_difference`
    result = symmetric_difference(&set1, &set2);
    printf("Diferenca simetrica entre Set A e Set B: ");
    print(&result);
    printf("\n");
    clear(&result);

    // Testando `isSubset` e `isSuperSet`
    printf("Set B e subconjunto de Set A? %s\n", isSubset(&set2, &set1) ? "Sim" : "Nao");
    printf("Set A e superconjunto de Set B? %s\n", isSuperSet(&set1, &set2) ? "Sim" : "Nao");
    printf("\n");

    // Testando `isDisjoint`
    printf("Set A e Set B sao disjuntos? %s\n", isDisjoint(&set1, &set2) ? "Sim" : "Nao");
    printf("\n");

    // Testando `copy`
    copy(&set1, &set2);
    printf("Copia do Set A para Set B: ");
    print(&set2);
    printf("\n");

    // Testando `pop`
    Element *removed = pop(&set2);
    if (removed != NULL)
    {
        printf("Elemento removido de Set B com pop: %d\n", removed->num);
        free(removed);
    }
    printf("Set B apos pop: ");
    print(&set2);
    printf("\n");

    // Limpando memoria de todos os conjuntos
    clear(&set1);
    clear(&set2);

    return 0;
}
