#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct Set Set;
typedef struct Element Element;

struct Element
{
    int num;
    Element *prox;
};

struct Set
{
    struct Element *topo;
};

void initSet();
bool isEmpty(Set *set);
int len(Set *set);
bool isIn(Set *set, int num);
bool isNotIn(Set *set, int num);
void add(Set *set, int num);
void removeSet(Set *set, int num);
void clear(Set *set);
void print(Set *set);
Set unionSet(Set *set1, Set *set2);
Set intersection(Set *set1, Set *set2);
Set difference(Set *set1, Set *set2);
Element *pop(Set *set);
bool isSubset(Set *set1, Set *set2);
bool isSuperSet(Set *set1, Set *set2);
void copy(Set *set1, Set *set2);
bool isDisjoint(Set *set1, Set *set2);
Set symmetric_difference(Set *set1, Set *set2);

#endif