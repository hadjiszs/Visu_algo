#ifndef _TYPE_
#define _TYPE_

typedef enum {BLANC, NOIR, GRIS} couleur_t;

typedef struct cellule {
     int val;
     struct cellule* pred;
     struct cellule* succ;     
}cellule_t;

typedef struct {
     cellule_t* tete;
}liste_t;

typedef struct sommet {
     couleur_t couleur;
     int f;
     int d;   
     int val;
     struct sommet* pere;
} sommet_t;

typedef struct {
     liste_t** dat;
     int nbrA;
     int nbrS;
} liste_adjacence_t;

typedef struct {
     int** dat;
     int nbrA;
     int nbrS;
} matrice_adjacence_t;

typedef struct {
     liste_adjacence_t* liste_adj;
     matrice_adjacence_t* matrice_adj;

     sommet_t** tabS;
     
     int A; // Nombre d'arrete
     int S; // Nombre noeuds
} graphe_t;

typedef struct {
     sommet_t** tab;
     int tete;
     int queue;
} file_t;

typedef struct {
     file_t* f;
} parcours_t;

#endif
