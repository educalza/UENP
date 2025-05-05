#include "header.h"

void initSet(Set *set) {
    set->topo = NULL;
}

bool isEmpty(Set *set) {
    return set->topo == NULL;
}

int len(Set *set) {
    int count = 0;
    Element *atual = set->topo;
    while (atual != NULL) {
        count++;
        atual = atual->prox;
    }
    return count;
}

bool isIn(Set *set, int num) {
    Element *atual = set->topo;
    while (atual != NULL) {
        if (atual->num == num) {
            return true;
        }
        atual = atual->prox;
    }
    return false;
}

bool isNotIn(Set *set, int num) {
    Element *atual = set->topo;
    while (atual != NULL) {
        if (atual->num == num) {
            return false;
        }
        atual = atual->prox;
    }
    return true;
}

void add(Set *set, int num) {
    if (isIn(set, num)) {
        return;
    }
    Element *newElement = (Element *)malloc(sizeof(Element));
    newElement->num = num;
    newElement->prox = set->topo;
    set->topo = newElement;
}

void removeSet(Set *set, int num) {
    if (isEmpty(set)) {
        return;
    }
    Element *atual = set->topo;
    Element *ant = NULL;
    while (atual != NULL) {
        if (atual->num == num) {
            if (ant == NULL) {
                set->topo = atual->prox;
            } else {
                ant->prox = atual->prox;
            }
            free(atual);
            return;
        }
        ant = atual;
        atual = atual->prox;
    }
}

void clear(Set *set) {
    Element *atual = set->topo;
    Element *ant = NULL;
    while (atual != NULL) {
        ant = atual;
        atual = atual->prox;
        free(ant);
    }
    set->topo = NULL;
}

void print(Set *set) {
    Element *atual = set->topo;
    printf("{");
    while (atual != NULL) {
        printf("%d, ", atual->num);
        atual = atual->prox;
    }
    printf("}\n");
}

Set unionSet(Set *set1, Set *set2) {
    Set uniao;
    initSet(&uniao);

    Element *atual = set1->topo;
    while (atual != NULL) {
        add(&uniao, atual->num);
        atual = atual->prox;
    }

    atual = set2->topo;
    while (atual != NULL) {
        add(&uniao, atual->num);
        atual = atual->prox;
    }

    return uniao;
}

Set intersection(Set *set1, Set *set2) {
    Set interseccao;
    initSet(&interseccao);

    Element *atual = set1->topo;
    while (atual != NULL) {
        if (isIn(set2, atual->num)) {
            add(&interseccao, atual->num);
        }
        atual = atual->prox;
    }
    return interseccao;
}

Set difference(Set *set1, Set *set2) {
    Set diferenca;
    initSet(&diferenca);

    Element *atual = set1->topo;
    while (atual != NULL) {
        if (isNotIn(set2, atual->num)) {
            add(&diferenca, atual->num);
        }
        atual = atual->prox;
    }
    return diferenca;
}

Element *pop(Set *set) {
    if (isEmpty(set)) {
        return NULL;
    }

    int qnt = len(set);

    srand(time(NULL));
    int random = rand() % qnt;

    Element *ant = NULL;
    Element *proxi = set->topo;
    for (int i = 0; i < random; i++) {
        ant = proxi;
        proxi = proxi->prox;
    }

    if (ant == NULL) {
        set->topo = proxi->prox;
    } else {
        ant->prox = proxi->prox;
    }

    proxi->prox = NULL;
    return proxi;
}

bool isSubset(Set *set1, Set *set2) {
    Element *atual = set1->topo;
    while (atual != NULL) {
        if (isNotIn(set2, atual->num)) {
            return false;
        }
        atual = atual->prox;
    }
    return true;
}

bool isSuperSet(Set *set1, Set *set2) {
    return isSubset(set2, set1);
}

void copy(Set *set1, Set *set2) {
    if (!isEmpty(set1)) {
        clear(set2);
    }
    Element *atual = set1->topo;
    while (atual != NULL) {
        add(set2, atual->num);
        atual = atual->prox;
    }
}

bool isDisjoint(Set *set1, Set *set2) {
    Element *atual = set1->topo;
    while (atual != NULL) {
        if (isIn(set2, atual->num)) {
            return false;
        }
        atual = atual->prox;
    }
    return true;
}

Set symmetric_difference(Set *set1, Set *set2) {
    Set diferenca1 = difference(set1, set2);
    Set diferenca2 = difference(set2, set1);

    Set symmetricDifferent = unionSet(&diferenca1, &diferenca2);

    clear(&diferenca1);
    clear(&diferenca2);

    return symmetricDifferent;
}
