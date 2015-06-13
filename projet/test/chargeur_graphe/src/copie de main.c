#include <stdio.h>
#include <stdlib.h>

#include "graphe.h"

#define BLANC (25)
#define GRIS  (31)
#define NOIR  (35)

/* Structure d'information de noeud */
typedef struct {
     int couleur;
     int distance;
     
}noeud_info_t;

int main(int argc, char* argv[])
{      
     time_t tfin, tdebut, ttotal;
     tdebut = time(NULL);

     liste_adjacence_t a = creerListesAdjacences(argv[1]);
       
     int nbr_noeud = a.nbr;
       
     /* Export pdf */
/*     FILE* fic = fopen("grapheMATtest.dot", "w");
     
     fprintf(fic, "graph G {");
     fprintf(fic, "\t node[shape=plaintext]");
     export_dot_m(fic, &m);
     fprintf(fic, "\n}");
     fclose(fic);*/
     /* Fin export pdf */

     /* Export pdf */     
/*     fic = fopen("grapheLISTtest.dot", "w");
     
     fprintf(fic, "graph G {");
     fprintf(fic, "\t node[shape=plaintext]");
     export_dot_l(fic, &a);
     fprintf(fic, "\n}");
     fclose(fic);*/
     /* Fin export pdf */     

     tfin = time(NULL);
     ttotal = difftime(tfin, tdebut);

     printf("\n-- Temps total: %d", (int)ttotal);

     return 0;
}
