#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "file.h"
#include "graphe.h"

int main(int argc, char* argv[])
{
     if(argc != 2) return EXIT_FAILURE;
     
     time_t tfin, tdebut, ttotal;
     tdebut = time(NULL);

     graphe_t* g = creerGraphe(argv[1]);

     // Parcours largeur
     afficherListesAdjacences(g->liste_adj);
     afficherMatriceAdjacences(g->matrice_adj);
     parcours_t* p = parcours_largeur(g, g->tabS[0]);

     afficher_file(p->f);          
     
     //parcours_t* p2 = parcours_profondeur(g);
     //afficherParcoursProfondeur(g);
     //afficher_file(p2->f);     
     
     int i;
     for(i = 0; i < g->S; i++)
     {
	  printf("\nPCC(%d -> %2d): ", 0, i);
	  afficher_chemin(g, g->tabS[0], g->tabS[i]);
     }
     
     tfin = time(NULL);
     ttotal = difftime(tfin, tdebut);

     printf("\nNombre sommets: %d", g->S);
     printf("\nNombre arretes: %d", g->A);

     detruireGraphe(g);
     
//   detruire_file(p->f);
//   free(p);
     
   //  detruire_file(p2->f);
     //free(p2);
     
     printf("\n-- Temps total: %d", (int)ttotal);
     
     return 0;
}
