#include <stdio.h>

#include "liste.hpp"

Liste::Liste()
{
     _tete = NULL;
}

Liste::~Liste()
{
     cellule_t* c = _tete;
     cellule_t* successeur = NULL;

     while(c != NULL)
     {
          successeur = c->succ;
          detruire_cellule(c);

          c = successeur;
     }
}

void Liste::inserer(cellule_t* newtete)
{
     if(_tete == NULL)
     {
          _tete = newtete;
          return;
     }

     cellule_t* anctete = _tete;

     newtete->pred = NULL;
     newtete->succ = anctete;
     anctete->pred = newtete;

     _tete = newtete;
}

cellule_t* Liste::rechercher(int val)
{
     if(_tete == NULL) return NULL; // Si le pointeur passé en parametre est NULL, on s'en va

     int trouver = 0;
     
     cellule_t* c = _tete;
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

int Liste::compter_liste()
{
     int res = 0;
     cellule_t* ci = _tete;
     
     while(ci != NULL)
     {
          res++;
          ci = ci->succ;
     }

     return res;
}

void Liste::afficher_liste()
{
     cellule_t* ci = _tete;

     while(ci != NULL)
     {
          printf(" %3d", ci->val);
          ci = ci->succ;
     }
}

/* Permet de détruire une cellule d'une liste en mettant à jour les redirections de pointeur */
void Liste::supprimer(cellule_t* c)
{
     if(c == NULL) return; // SI le pointeur passé en parametre est NULL, on s'en va

     if(_tete == c) // Si la cellule est tete de liste
     {
          c->succ->pred = NULL;
          _tete = c->succ;
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
