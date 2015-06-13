#include "arrete.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

arrete* creerArrete(int a, int b, int c)
{
    arrete* ar = (arrete*)malloc(sizeof(arrete));
    ar->s1 = a;
    ar->s2 = b;
    ar->p = c;
    ar->prec = NULL;
    ar->suiv = NULL;
    return ar;
}

void detruireArrete(arrete** a)
{
    if(a != NULL)
    {
        if(*a != NULL)
        {
            free(*a);
        }
    }
}

void afficherArrete(arrete* a)
{
    if(a != NULL)
    {
        printf("\nArrete: (%d,%d)  %d\n",a->s1,a->s2,a->p);
    }
}

void afficherArretes(arrete** ar, int n)
{
    int i;
    for(i = 0; i<n; i++)
    {
        afficherArrete(ar[i]);
    }
}


void triArretes(arrete** a, int n)
{
    if(n < 2)
    {
        return;
    }
    int p = a[n/2]->p;
    arrete** l = a;
    arrete** r = a + n -1;
    while(l <= r)
    {
        if((*l)->p < p)
        {
            l++;
        }
        else if((*r)->p > p)
        {
            r--;
        }
        else
        {
            arrete* t = *l;
            *l = *r;
            *r = t;
            l++;
            r--;
        }
    }
    triArretes(a, r - a + 1);
    triArretes(l, a + n - l);
}
