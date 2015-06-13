#ifndef _LISTE_
#define _LISTE_

#include <string.h>
#include "cellule.h"

#include "type.h"

liste_t* creer_liste();
void detruire_liste(liste_t**);
void inserer(liste_t*, cellule_t*);
cellule_t* rechercher(liste_t*, int);
void supprimer(liste_t*, cellule_t*);
int compter_liste(liste_t*);
void afficher_liste(liste_t*);

#endif
