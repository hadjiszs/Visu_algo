#include <stdio.h>

#include "liste.h"

liste_t* creer_liste()
{     
     liste_t* l = (liste_t*) malloc(sizeof(liste_t));
     
     if(l != NULL)
	  l->tete = NULL;

     return l;
}

void detruire_liste(liste_t** l)
{
     if(*l == NULL) return; // Si le pointeur passé en parametre est NULL, on s'en va

     cellule_t* c = (*l)->tete;
     cellule_t* successeur = NULL;

     while(c != NULL)
     {
	  successeur = c->succ;
	  detruire_cellule(c);
	  
	  c = successeur;
     }

     free(*l);
     *l = NULL;
}

void inserer(liste_t* l, cellule_t* newtete)
{
     if(l == NULL) return;

     if(l->tete == NULL)
     {
	  l->tete = newtete;
	  return;
     }
     
     cellule_t* anctete = l->tete;
     
     newtete->pred = NULL;
     newtete->succ = anctete;
     anctete->pred = newtete;
     
     l->tete = newtete;
}

cellule_t* rechercher(liste_t* l, int val)
{
	if(l == NULL) return NULL;
	
     if(l->tete == NULL) return NULL; // Si le pointeur passé en parametre est NULL, on s'en va
     
     int trouver = 0;
     
     cellule_t* c = l->tete;
     cellule_t* successeur = NULL;

     while(!trouver && c != NULL)
     {
	  successeur = c->succ;
	  if(c->val == val)
	       trouver = 1;
	  else
	       c = successeur;
     }

     // On retourne c, qui sera soit la cellule trouvé, soit NULL si pas présent
     return c;
}

int compter_liste(liste_t* l)
{
     if(l == NULL) return -1;
     
     int res = 0;
     cellule_t* ci = l->tete;
     
     while(ci != NULL)
     {
	  res++;
	  ci = ci->succ;
     }

     return res;
}

void afficher_liste(liste_t* l)
{
     if(l == NULL) return;
     
     cellule_t* ci = l->tete;
     
     while(ci != NULL)
     {
	  printf(" %3d", ci->val);
	  ci = ci->succ;
     }
}

/* Permet de détruire une cellule d'une liste en mettant à jour les redirections de pointeur */
void supprimer(liste_t* l, cellule_t* c)
{
     if(c == NULL) return; // SI le pointeur passé en parametre est NULL, on s'en va            

     if(l->tete == c) // Si la cellule est tete de liste
     {
	  c->succ->pred = NULL;
	  l->tete = c->succ;
	  detruire_cellule(c);
     }
     else if(c->succ == NULL) // Si la cellule est queue
     {
	  c->pred->succ = NULL;
	  detruire_cellule(c);
     }
     else // Si la cellule a un pred et un succ
     {
	  c->pred->succ = c->succ;	// On fait pointer le succ du pred sur le succ     
	  c->succ->pred = c->pred;	// On fait pointer le pred du succ sur le pred
	  detruire_cellule(c);
     }
}
