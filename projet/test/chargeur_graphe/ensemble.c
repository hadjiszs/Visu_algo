#include "ensemble.h"
#include <stdio.h>
#include <stdlib.h>

ensemble* creerEnsemble(sommet* x)
{
    ensemble* e = (ensemble*)malloc(sizeof(ensemble));
    e->l = creerListe();
    x->ensemble = (void*)e;
    cellule* c = creerCelluleSommet(x);
    inserer(e->l,c);
    return e;
}

sommet* trouverEnsemble(sommet* x)
{
    ensemble* e = (ensemble*)x->ensemble;
    return e->l->deb->s;
}

void unionEnsemble(sommet* x, sommet* y)
{
    if(x != NULL && y != NULL)
    {
        ensemble* e1 = (ensemble*)x->ensemble;
        ensemble* e2 = (ensemble*)y->ensemble;

        liste* l1 = e1->l;
        liste* l2 = e2->l;

        cellule* temp = l2->deb;
        while(temp != NULL)
        {
            temp->s->ensemble = e1;
            temp = temp->suiv;
        }

        l1->fin->suiv = l2->deb;
        l2->deb->prec = l1->fin;
        l1->fin = l2->fin;
    }

}

void afficherEnsemble(ensemble* e)
{
    afficherListe(e->l);
}

void detruireEnsemble(ensemble* e)
{
    detruireListePartielle(&(e->l));
}
