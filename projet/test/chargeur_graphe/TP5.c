#include <stdlib.h>
#include "outilsGraphe.h"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
	 printf("Erreur: veuillez passer le nom du fichier correspondant au graphe");
	 exit(-1);
    }
    
    graphe* g = creerGraphe(argv[1]);
    afficherMatriceAdjacences(g);	
    afficherListesAdjacences(g);
    
    printf("[PARCOURS PROFONDEUR]");
    sommet** prof = parcoursProfondeurListe(g);
    afficherDate(prof, g->n);
    
    printf("[PARCOURS EN LARGEUR]");
    sommet** larg = parcoursLargeurListe(g, 0);
    
    afficherChemin(g, 
    
    return 0;
}