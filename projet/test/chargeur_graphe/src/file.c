#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "file.h"

#define MAXIMUM_FILE 200000

file_t* creer_file()
{
     file_t* f = (file_t*) malloc(sizeof(file_t));
     
     f->tab = (sommet_t**) malloc(sizeof(sommet_t*)*MAXIMUM_FILE);
     f->tete = 0;
     f->queue = 0;
     
     return f;
}

void detruire_file(file_t* f)
{
     if(f == NULL) return;

     if(f->tab != NULL)
	  free(f->tab);

     free(f);
}

int file_vide(file_t* f)
{
     if(f == NULL) return 0;
     
     return f->tete == f->queue;
}

void enfiler(file_t* f, sommet_t* s)
{
     if(f == NULL || s == NULL ) return;

     f->tab[f->queue] = s;

     if(f->queue == MAXIMUM_FILE)
	  f->queue = 1;
     else
	  f->queue++;
}

sommet_t* defiler(file_t* f)
{
     if(f == NULL || file_vide(f) == 1) return NULL;
     
     sommet_t* res = f->tab[f->tete];
//     printf("\n[%d]", f->tete);
     
     if(f->tete == MAXIMUM_FILE)
	  f->tete = 1;
     else
	  f->tete++;
     
     return res;
}

file_t* copier_file(file_t* f)
{
     if(f == NULL) return NULL;
     
     file_t* res = creer_file();
     
     memcpy(res->tab, f->tab, sizeof(sommet_t*)*MAXIMUM_FILE);

     res->tete = f->tete;
     res->queue = f->queue;
     
     return res;
}

void afficher_file(file_t* f)
{
     if(f == NULL) return;

     if(file_vide(f) == 1) printf("\n_file vide");

     file_t* copie = copier_file(f);
     
     printf("\nfile: ");
     
     sommet_t* tmp = NULL;
     while(! file_vide(copie))
     {
	  tmp = defiler(copie);
	  printf("\t%2d", tmp->val);
     }
     
     free(copie->tab);
     free(copie);
}
