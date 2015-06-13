#ifndef _GRAPHE_H_
#define _GRAPHE_H_
#include "file.h"
#include "listeArrete.h"

struct graphe
{
    liste** l;
    int** m;
    int n;
    int o;
    int v;
    int c;
};

typedef struct graphe graphe;

liste** creerListesAdjacences(char* f, int* n);
int** creerMatriceAdjacences(char* f, int* n);
void afficherMatriceAdjacences(graphe* g);
void afficherListesAdjacences(graphe* g);
graphe* creerGraphe(char* f);
void detruireGraphe(graphe** g);

#endif