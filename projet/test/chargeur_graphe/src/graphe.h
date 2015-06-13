#ifndef _GRAPHE_
#define _GRAPHE_

#include "liste.h"
#include "type.h"

liste_adjacence_t* creerListesAdjacences(char* path);
matrice_adjacence_t* creerMatriceAdjacences(char* path);

parcours_t* parcours_largeur(graphe_t* g, sommet_t* s); // parcours largeur en partant du noeud s
parcours_t* parcours_profondeur(graphe_t* g); // parcours en profondeur de g
void afficherParcoursProfondeur(graphe_t* g);
void DFS_visiter_noeud(graphe_t* g, sommet_t* u, int* time, parcours_t* p);
void export_dot_l(FILE* f, liste_adjacence_t* l);
void export_dot_m(FILE* f, matrice_adjacence_t* m);
void afficherListesAdjacences(liste_adjacence_t*);
void afficherMatriceAdjacences(matrice_adjacence_t*);
void afficher_chemin(graphe_t* g, sommet_t* i, sommet_t* j);

graphe_t* creerGraphe(char* path);
void detruireGraphe(graphe_t*);

#endif
