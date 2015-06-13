#include "cellule.h"

/* Créer une cellule avec une copie profonde */
cellule_t* creer_cellule(int val)
{
     cellule_t* c = (cellule_t*) malloc(sizeof(cellule_t));

     c->val = val;
     c->pred = NULL;
     c->succ = NULL;

     return c;
}

/* Permet de détruire une cellule sans prise en compte des indirections */
void detruire_cellule(cellule_t* c)
{    
     if(c == NULL) return; // Si le pointeur passé en parametre est NULL, on s'en va
     
     c->pred = NULL; // pour eviter effets de bords
     c->succ = NULL; // pour eviter effets de bords

     free(c);
}
