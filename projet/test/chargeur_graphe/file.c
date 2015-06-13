#include "file.h"
#include <stdio.h>
#include <stdlib.h>


file* creerFile()
{
    file* f = (file*)malloc(sizeof(file));
    f->l = creerListe();
    return f;
}

void detruireFile(file** f)
{
    if(f != NULL)
    {
        if(*f != NULL)
        {
            detruireListePartielle(&((*f)->l));
            free(*f);
        }
    }

}

int fileVide(file* f)
{
    return f->l->deb == NULL;
}

void enfiler(file* f, sommet* s)
{
    cellule* c = creerCelluleSommet(s);
    inserer(f->l,c);
}

sommet* defiler(file* f)
{
    if(f->l->fin ==NULL)
    {
        return NULL;
    }
    else
    {

        sommet* s = f->l->fin->s;
        if(f->l->fin->prec!= NULL)
        {
            f->l->fin->prec->suiv = NULL;
            f->l->fin = f->l->fin->prec;
        }
        else
        {
            f->l->deb = NULL;
            f->l->fin = NULL;
        }
        return s;
    }
}

