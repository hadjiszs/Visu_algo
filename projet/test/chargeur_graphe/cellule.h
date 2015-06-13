#ifndef _CELLULE_H_
#define _CELLULE_H_
#include "sommet.h"

struct cellule
{
    struct cellule* suiv;
    struct cellule* prec;
    int n;
    int p;
    sommet* s;
};

typedef struct cellule cellule;

cellule* creerCelluleSommet(sommet* s);
cellule* creerCellule(int n, int p);
void afficherCellule(cellule* c);
void detruireCellule(cellule** c);

#endif // _CELLULE_H_
