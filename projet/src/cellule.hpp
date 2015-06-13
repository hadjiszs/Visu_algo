#ifndef _CELLULE_
#define _CELLULE_

#include <stdlib.h>
#include <string.h>

typedef struct cellule {
     int val;
     struct cellule* pred;
     struct cellule* succ;     
} cellule_t;

cellule_t* creer_cellule(int val);
void detruire_cellule(cellule_t* c);

#endif
