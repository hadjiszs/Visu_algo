#include "graphe.h"
#include "tas.h"
#include "file.h"
#include "listeArrete.h"
#include "ensemble.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define inf INT_MAX


liste** creerListesAdjacences(char* f, int* n)
{
    FILE* fichier = fopen(f, "r");
    cellule* c_ = NULL;
    int temp1;
    int temp2;
    int o = 0;
    int v = 0;
    int c = 0;
    char* s = (char*)malloc(sizeof(char)*50);
    fscanf(fichier,"%s",s);
    fscanf(fichier,"%d",n);
    liste** l =(liste**)malloc(sizeof(liste*)*(*n));
    int i;
    int temp3 = 0;
    for(i = 0; i<*n; i++)
    {
        l[i] = creerListe();
    }

    fscanf(fichier,"%s",s);
    fscanf(fichier,"%d",&o);
    fscanf(fichier,"%s",s);
    fscanf(fichier,"%d",&v);
    fscanf(fichier,"%s",s);
    fscanf(fichier,"%d",&c);
    fscanf(fichier,"%s",s);
    while(fscanf(fichier,"%s",s)!=-1)
    {
        if(strcmp(s,"finDefAretes")!=0)
        {
            temp1 = atoi(s);
            fscanf(fichier,"%d",&temp2);
            if(v)
            {
                fscanf(fichier,"%d",&temp3);
            }

            if(o)
            {
                if(v)
                {
                    c_ = creerCellule(temp2,temp3);
                    inserer(l[temp1],c_);
                }
                else
                {
                    c_ = creerCellule(temp2,1);
                    inserer(l[temp1],c_);
                }
            }
            else
            {
                if(v)
                {
                    c_ = creerCellule(temp2,temp3);
                    inserer(l[temp1],c_);
                    c_ = creerCellule(temp1,temp3);
                    inserer(l[temp2],c_);
                }
                else
                {
                    c_ = creerCellule(temp2,1);
                    inserer(l[temp1],c_);
                    c_ = creerCellule(temp1,1);
                    inserer(l[temp2],c_);
                }

            }
        }
    }
    close(f);
    return l;
}


int** creerMatriceAdjacences(char* f,int* n)
{
    FILE* fichier = fopen(f, "r");
    int temp1;
    int temp2;
    int o = 0;
    int v = 0;
    int c = 0;
    char* s = (char*)malloc(sizeof(char)*50);
    fscanf(fichier,"%s",s);
    fscanf(fichier,"%d",n);
    fscanf(fichier,"%s",s);
    fscanf(fichier,"%d",&o);
    fscanf(fichier,"%s",s);
    fscanf(fichier,"%d",&v);
    fscanf(fichier,"%s",s);
    fscanf(fichier,"%d",&c);
    int** tab = (int**)malloc(sizeof(int*)*(*n));
    int i;
    int j;
    for(i=0; i<*n; i++)
    {
        tab[i] = (int*)malloc(sizeof(int)*(*n));
        for(j=0; j<*n; j++)
        {
            tab[i][j]=0;
        }
    }
    int temp3 = 0;
    fscanf(fichier,"%s",s);
    while(fscanf(fichier,"%s",s)!=-1)
    {
        if(strcmp(s,"finDefAretes")!=0)
        {
            temp1 = atoi(s);
            fscanf(fichier,"%d",&temp2);
            if(v)
            {
                fscanf(fichier,"%d",&temp3);
            }
            if(o)
            {
                if(v)
                {
                    tab[temp1][temp2] = temp3;
                }
                else
                {
                    tab[temp1][temp2] = 1;
                }
            }
            else
            {
                if(v)
                {
                    tab[temp1][temp2] = temp3;
                    tab[temp2][temp1] = temp3;
                }
                else
                {
                    tab[temp1][temp2] = 1;
                    tab[temp2][temp1] = 1;
                }

            }
        }
    }
    close(f);
    return tab;
}

/*
 * Permet d'afficher la matrice d'adjacence
 */
void afficherMatriceAdjacences(graphe* g)
{
     if(g == NULL) return;
     
     int i, j, k;

     int nbr_sep = 8 + 4*g->n;
     char* sep = (char*) malloc(sizeof(char)*(nbr_sep + 1));
     
     for(k = 0; k < nbr_sep; k++) sep[k] = '-';

     sep[nbr_sep] = '\0';
     
     printf("\n==================== "
	    "\n// Matrice adjacence"
	    "\n%s"
	    "\n       |", sep);
     
     for(i = 0; i < g->n; i++)
	  printf(" %-2d|", i);

     for(i = 0; i < g->n; i++)
     {
	  
	  printf("\n%s"
		 "\n[%2d] - |", sep, i);
	  
	  for(j = 0; j < g->n; j++)
	       if(g->m[i][j] != 1)
		    printf("\033[41;01m%2d \033[00m|", g->m[i][j]);
	       else
		    printf("\033[42;01m%2d \033[00m|", g->m[i][j]);
     }
     printf("\n%s", sep);

     free(sep);
}

void afficherListesAdjacences(graphe* g)
{
     if(g == NULL) return;
     
     int i = 0;

     printf("\n================== "
	    "\n// Liste adjacence"
	    "\n------------------");
     
     for(i = 0; i < g->n; i++)
     {
	  printf("\n[%2d] -> ", i);
	  afficherListe(g->l[i]);	  
     }
     
     printf("\n------------------");
}

graphe* creerGraphe(char* f)
{
    graphe* g = (graphe*)malloc(sizeof(graphe));
    int* n = (int*)malloc(sizeof(int));
    g->l = creerListesAdjacences(f,n);
    g->m = creerMatriceAdjacences(f,n);
    g->n = *n;
    return g;

}

void detruireGraphe(graphe** g)
{
    if(g != NULL)
    {
        if(*g != NULL)
        {
            if((*g)->l != NULL)
            {
                detruireListePartielle((*g)->l);
            }
            if((*g)->m != NULL)
            {
                free((*g)->m);
            }
            free(*g);
        }
    }
}

