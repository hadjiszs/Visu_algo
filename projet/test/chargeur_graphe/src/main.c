#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "file.h"
#include "graphe.h"

#define DEBUG

int main(int argc, char* argv[])
{
     if(argc != 2) return EXIT_FAILURE;
     
     time_t tfin, tdebut, ttotal;
     tdebut = time(NULL);

     graphe_t* g = creerGraphe(argv[1]);

     //afficherMatriceAdjacences(g->matrice_adj);

     FILE* fic = fopen("expor", "w");
     export_dot_m(fic, g->matrice_adj);
     fclose(fic);
     
     tfin = time(NULL);
     ttotal = difftime(tfin, tdebut);

     printf("\nNombre sommets: %d", g->S);
     printf("\nNombre arretes: %d", g->A);

     detruireGraphe(g);
     
     printf("\n-- Temps total: %d", (int)ttotal);
     
     return 0;
}
