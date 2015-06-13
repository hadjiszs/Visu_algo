#include "cellule.h"
#include <stdio.h>
#include <stdlib.h>

cellule* creerCelluleSommet(sommet* s)
{
    cellule* c = (cellule*)malloc(sizeof(cellule));
    c->suiv = NULL;
    c->prec = NULL;
    c->s = s;
    c->n = s->v;
    c->p = 1;
    return c;
}

cellule* creerCellule(int n, int p)
{
    cellule* c = (cellule*)malloc(sizeof(cellule));
    c->suiv = NULL;
    c->prec = NULL;
    c->s = NULL;
    c->n = n;
    c->p = p;
    return c;
}

void afficherCellule(cellule* c)
{
    if(c != NULL)
    {
            printf("\nSommet: %d Poids: %d\n",c->n,c->p);
    }
}

void detruireCellule(cellule** c)
{
    if(c != NULL)
    {
        if(*c != NULL)
        {
            if((*c)->s != NULL)
            {
                free(&((*c)->s));
            }
            free(*c);
        }
    }
}
