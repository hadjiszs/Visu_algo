#include <stdio.h>
#include <string.h>
#include "graphe.h"

#include "file.h"

/*
 * Création du graphe par liste d'adjacence
 */
liste_adjacence_t* creerListesAdjacences(char* path)
{
     FILE* fichier = NULL;
     
     /* On ouvre le fichier en lecture seule ici, on sort si il y a echec à l'ouverture */
     if((fichier = fopen(path, "r")) == NULL)
     {
          printf("ERREUR! Echec à l'ouverture du fichier");
          exit(EXIT_FAILURE);
     }

     /* Si on est ici alors le fichier est ouvert */
     liste_adjacence_t* res = (liste_adjacence_t*) malloc(sizeof(liste_adjacence_t));
     
     int  nArrete = 0,
	  nSommets = 0,
	  oriente = 0,
	  value = 0,
	  complet = 0;	  
     
     char buff[64];
     
     fscanf(fichier, "%s %d", buff, &nSommets);
     fscanf(fichier, "%s %d", buff, &oriente);
     fscanf(fichier, "%s %d", buff, &value);
     fscanf(fichier, "%s %d", buff, &complet);

     res->dat = (liste_t**) malloc(sizeof(liste_t*)*nSommets);

     int i;
     for(i = 0; i < nSommets; i++)
	  res->dat[i] = creer_liste();
     
     int nb1, nb2;

     fscanf(fichier, "%s", buff);
     if(strcmp(buff, "debutDefAretes") == 0)
     {
	  while(fgets(buff, 16, fichier) != 0 && strncmp(buff, "finDefAretes", 12) != 0)
	  {
	       if(strlen(buff) < 3) continue; // On peut pas etre en dessous de 3 caracteres pour une ligne
	       
	       // Ici buff est composé de 2 nombres
	       sscanf(buff, "%d %d", &nb1, &nb2);
               if(nb1 != nb2)
               {
                    nArrete++;
#ifdef DEBUG
                    printf("\n-------\n[BUFF]");
                    printf(" %s", buff);
                    printf("\n[AJOUT] %d => %d", nb1, nb2);
#endif

                    // On ajoute la cellule contenant nb2 dans la liste numéro nb1
                    if(rechercher(res->dat[nb1], nb2) == NULL)
                         inserer(res->dat[nb1], creer_cellule(nb2));
                    
                    // On ajoute la cellule contenant nb2 dans la liste numéro nb1	
                    if(rechercher(res->dat[nb2], nb1) == NULL)
                         inserer(res->dat[nb2], creer_cellule(nb1));
               }
	  }
     }
     else
	  printf("Erreur structure fichier: %s", buff);
          
     fclose(fichier);

     res->nbrS = nSommets;
     res->nbrA = nArrete;

     return res;
}

/*
 * Création du graphe par matrice d'adjacence
 */
matrice_adjacence_t* creerMatriceAdjacences(char* path)
{
     FILE* fichier = NULL;
     
     /* On ouvre le fichier en lecture seule ici, on sort si il y a echec à l'ouverture */
     if((fichier = fopen(path, "r")) == NULL)
     {
          printf("ERREUR! Echec à l'ouverture du fichier");
          exit(EXIT_FAILURE);
     }

     /* Si on est ici alors le fichier est ouvert */
     matrice_adjacence_t* res = (matrice_adjacence_t*) malloc(sizeof(matrice_adjacence_t));
     
     int  nArrete = 0,
	  nSommets = 0,
	  oriente = 0,
	  value = 0,
	  complet = 0;	  
     
     char buff[64];
     
     fscanf(fichier, "%s %d", buff, &nSommets);
     fscanf(fichier, "%s %d", buff, &oriente);
     fscanf(fichier, "%s %d", buff, &value);
     fscanf(fichier, "%s %d", buff, &complet);

     /* Allocation du tableau 2D */
     res->dat = (int**) malloc(sizeof(int*)*nSommets);
     int i;
     for(i = 0; i < nSommets; i++)
	  res->dat[i] = (int*) malloc(sizeof(int)*nSommets);

     /* Initialisation */
     int j;
     for(i = 0; i < nSommets; i++)
	  for(j = 0; j < nSommets; j++)
	       res->dat[i][j] = 0;
     
     int nb1, nb2;

     fscanf(fichier, "%s", buff);
     if(strcmp(buff, "debutDefAretes") == 0)
     {
	  while(fgets(buff, 16, fichier) != 0 && strncmp(buff, "finDefAretes", 12) != 0)
	  {
	       if(strlen(buff) < 3) continue; // On peut pas etre en dessous de 3 caractere pour une ligne
	       
	       // Ici buff est composé de 2 nombres
	       sscanf(buff, "%d %d", &nb1, &nb2);
#ifdef DEBUG
	       printf("\n-------\n[BUFF]");
	       printf(" %s", buff);
	       printf("\n[AJOUT] %d => %d", nb1, nb2);
#endif
               if(nb1 != nb2)
               {
                    res->dat[nb1][nb2] = -1;
                    res->dat[nb2][nb1] = -1;
               }
	  }
     }
     else
	  printf("Erreur structure fichier: %s", buff);
     
     fclose(fichier);

     res->nbrS = nSommets;
     res->nbrA = nArrete;

     int* nbrdif = (int*) malloc(sizeof(int)*nSommets);

     for(i = 0; i < nSommets; i++)
     {
          nbrdif[i] = 0;
          
          for(j = 0; j < nSommets; j++)
          {
               if(res->dat[i][j] == -1)
                    nbrdif[i]++;
          }
     }
          
     for(i = 0; i < nSommets; i++)
          res->dat[i][i] = nbrdif[i];

     free(nbrdif);
     
     return res;
}

/*
 * Renvoie le code en langage DOT de l'arbre
 */
void export_dot_l(FILE* f, liste_adjacence_t* l)
{
     if(l == NULL) return;

     int i = 0;
     liste_t* list_courante = NULL;
     cellule_t* cell_courante = NULL;

     for(i = 0; i < l->nbrS; i++)
     {
	  list_courante = l->dat[i];
	  cell_courante = list_courante->tete;
	  
	  while(cell_courante != NULL)
	  {
	       fprintf(f, "\n\t%d -- %d;", i, cell_courante->val);
	       cell_courante = cell_courante->succ;
	  }	  
     }
}

/*
 * Renvoie le code en langage DOT de l'arbre
 */
void export_dot_m(FILE* f, matrice_adjacence_t* m)
{
     if(m == NULL) return;
     
     int i, j;

     fprintf(f, " [ \n");
     
     for(i = 1; i < m->nbrS; i++)
     {          
	  for(j = 1; j < m->nbrS; j++)
               fprintf(f, " %d ", m->dat[i][j]);

          if(i == m->nbrS - 1)
               fprintf(f, " ]\n");
          else
               fprintf(f, " ;\n");
     }
}

/*
 * Permet d'afficher la liste d'adjacence
 */
void afficherListesAdjacences(liste_adjacence_t* l)
{
     if(l == NULL) return;
     
     int i = 0;

     printf("\n================== "
	    "\n// Liste adjacence"
	    "\n------------------");
     
     for(i = 0; i < l->nbrS; i++)
     {
	  printf("\n[%2d] -> ", i);
	  afficher_liste(l->dat[i]);	  
     }
     printf("\n------------------");
}

/*
 * Permet d'afficher la matrice d'adjacence
 */
void afficherMatriceAdjacences(matrice_adjacence_t* m)
{
     if(m == NULL) return;
     
     int i, j, k;

     int nbr_sep = 8 + 4*m->nbrS;
     char* sep = (char*) malloc(sizeof(char)*(nbr_sep + 1));
     
     for(k = 0; k < nbr_sep; k++) sep[k] = '-';

     sep[nbr_sep] = '\0';
     
     printf("\n==================== "
	    "\n// Matrice adjacence"
	    "\n%s"
	    "\n       |", sep);
     
     for(i = 0; i < m->nbrS; i++)
	  printf(" %-2d|", i);

     for(i = 0; i < m->nbrS; i++)
     {
	  
	  printf("\n%s"
		 "\n[%2d] - |", sep, i);
	  
	  for(j = 0; j < m->nbrS; j++)
	       if(m->dat[i][j] == 1)
		    printf("\033[42;01m%2d \033[00m|", m->dat[i][j]);
	       else
		    printf("\033[41;01m%2d \033[00m|", m->dat[i][j]);
     }
     printf("\n%s", sep);

     free(sep);
}

/*
 * Creation d'un graphe
 */
graphe_t* creerGraphe(char* path)
{
     graphe_t* g = (graphe_t*) malloc(sizeof(graphe_t));

     g->liste_adj = creerListesAdjacences(path);
     g->matrice_adj = creerMatriceAdjacences(path);

     g->S = g->liste_adj->nbrS;
     g->A = g->liste_adj->nbrA;

     g->tabS = (sommet_t**) malloc(sizeof(sommet_t*)*g->S);

     int i = 0;
     for(i = 0; i < g->S; i++)
     {
	  g->tabS[i] = (sommet_t*) malloc(sizeof(sommet_t));
	  
	  g->tabS[i]->couleur = BLANC;
	  g->tabS[i]->pere = NULL;
	  g->tabS[i]->val = i;
	  g->tabS[i]->d = 0;
     }
     
     return g;
}

/*
 * Destruction d'un graphe
 */
void detruireGraphe(graphe_t* g)
{
     if(g == NULL) return;
     
     int i = 0;

     // Liberation memoire de la liste des sommets
     for(i = 0; i < g->S; i++)
	  free(g->tabS[i]);

     free(g->tabS);
     
     // Liberation memoire liste
     if(g->liste_adj != NULL)
     {
	  for(i = 0; i < g->S; i++)
	       detruire_liste(&g->liste_adj->dat[i]);
     
	  free(g->liste_adj->dat);
	  free(g->liste_adj);
     }
     
     // Liberation memoire matrice
     if(g->liste_adj != NULL)
     {
	  for(i = 0; i < g->S; i++)
	       free(g->matrice_adj->dat[i]);
	  
	  free(g->matrice_adj->dat);
	  free(g->matrice_adj);
     }
     
     // Liberation du graphe
     free(g);
}

/*
 * Parcours en largeur d'un graphe
 */
parcours_t* parcours_largeur(graphe_t* g, sommet_t* s)
{
     parcours_t* p = (parcours_t*) malloc(sizeof(parcours_t));
     
     p->f = creer_file();

     // Initialisation
     int i = 0;
     sommet_t* u = NULL;
     sommet_t* v = NULL;
     for(i = 0; i < g->S; i++)
     {
	  u = g->tabS[i];
	  
	  if(s == u) continue;
	  
	  u->couleur = BLANC;
	  u->pere = NULL;
	  u->val = i;
	  u->d = 0;
     }

     s->couleur = GRIS;
     s->d = 0;
     s->pere = NULL;

     cellule_t* c = NULL;
     liste_t* ladj = NULL;
     file_t* F = creer_file();
     
     enfiler(F, s);
     enfiler(p->f, s);
     
     while(! file_vide(F))
     {
	  u = defiler(F);
	  
	  // recuperation de la liste d'adjacence
	  ladj = g->liste_adj->dat[u->val];
	  
	  c = ladj->tete;
	  while(c != NULL)
	  {
	       v = g->tabS[c->val];
	       if(v->couleur == BLANC)
	       {
		    v->couleur = GRIS;
		    v->d = u->d + 1;
		    v->pere = u;
		    enfiler(F, v);
		    enfiler(p->f, v);
	       }
	       
	       c = c->succ;
	  }

	  u->couleur = NOIR;
     }

     detruire_file(F);
     
     return p;
}

void afficher_chemin(graphe_t* g, sommet_t* i, sommet_t* j)
{
     if(g == NULL || i == NULL || j == NULL) return;
     
     if(i->val == j->val)
	  printf("\t[%d]", i->val);
     else if(j->pere == NULL)
	  printf("\t no(%d;%d)", i->val, j->val);
     else
     {
	  afficher_chemin(g, i, j->pere);
	  printf("\t[%d]", j->val);
     }
}

void afficherParcoursProfondeur(graphe_t* g)
{
     int i;
     for(i = 0; i < g->S; i++)
     {
	  printf("\n[%2d] {D: %2d ; F: %2d}", i, g->tabS[i]->d, g->tabS[i]->f);
     }
}

parcours_t* parcours_profondeur(graphe_t* g)
{
     if(g == NULL) return NULL;
     
     parcours_t* p = (parcours_t*) malloc(sizeof(parcours_t));
     p->f = creer_file();

     sommet_t* u = NULL;
     int date = 0;
     
     int i;    
     for(i = 0; i < g->S; i++)
     {
	  u = g->tabS[i];
	  u->couleur = BLANC;
	  u->pere = NULL;
     }

     date = 0;
     for(i = 0; i < g->S; i++)
     {
	  u = g->tabS[i];
	  if(u->couleur == BLANC)
	       DFS_visiter_noeud(g, u, &date, p);
     }
		    
     return p;
}

void DFS_visiter_noeud(graphe_t* g, sommet_t* u, int* time, parcours_t* p)
{
     enfiler(p->f, u);
     
     *time += 1;

     u->d = *time;
     u->couleur = GRIS;

     sommet_t* v = NULL;
     liste_t* ladj = NULL;
     cellule_t* c = NULL;     
     // recuperation de la liste d'adjacence
     ladj = g->liste_adj->dat[u->val];
     
     c = ladj->tete;
     while(c != NULL)
     {
	  v = g->tabS[c->val];
	  if(v->couleur == BLANC)
	  {
	       v->pere = u;
	       DFS_visiter_noeud(g, v, time, p);
	  }
	  
	  c = c->succ;
     }

     u->couleur = NOIR;
     *time += 1;
     u->f = *time;
}
