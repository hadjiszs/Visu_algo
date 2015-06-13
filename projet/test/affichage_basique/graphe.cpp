/**
 * \file graphe.cpp
 * \brief Implementation de graphe.hpp
 */
#include <cstdio>
#include <string>

#include "graphe.hpp"
#include "file.hpp"

/*
 * Creation d'un graphe
 */
Graphe::Graphe(std::string path)
     : Structure(path)
{
     creer_listes_adjacences();
     creer_matrice_adjacences();
     creer_matrice_laplace();
     
     _S = _liste_adj->nbrS;
     std::cout << "Nombre de sommets: " << _S << std::endl;
     _A = _liste_adj->nbrA;
     
     _tabS = (sommet_t**) malloc(sizeof(sommet_t*)*_S);
     
     int i = 0;
     for(i = 0; i < _S; i++)
     {
          _tabS[i] = (sommet_t*) malloc(sizeof(sommet_t));
          
          _tabS[i]->couleur = BLANC;
          _tabS[i]->pere = NULL;
          _tabS[i]->val = i;
          _tabS[i]->d = 0;
     }
}

void Graphe::charger()
{
     std::cout << "\nAccès au fichier: [" << _path_fichier << "] [OK]" << std::endl;
     
     int nSommets = _S;
     double* gsldata = (double*) malloc(sizeof(double)*nSommets*nSommets);
     
     parcours_t* p = parcours_profondeur();
     afficher_parcours_profondeur();
     
#ifdef AFFDEBUG
     afficher_matrice_laplace();    
#endif
     
     //
     // Calcul valeur/vecteur propre ordre croissant
     //
     for(int i = 0; i < nSommets; i++)
          for(int j = 0; j < nSommets; j++)
               gsldata[i*nSommets + j] = _matrice_lap->dat[i][j];
     
     gsl_matrix_view m = gsl_matrix_view_array (gsldata, nSommets, nSommets);
     
     gsl_vector *eval = gsl_vector_alloc(nSommets);
     gsl_matrix *evec = gsl_matrix_alloc(nSommets, nSommets);
     
     gsl_eigen_symmv_workspace * w = gsl_eigen_symmv_alloc(nSommets);
     
     gsl_eigen_symmv (&m.matrix, eval, evec, w);
     gsl_eigen_symmv_free (w);
     gsl_eigen_symmv_sort (eval, evec,
                           GSL_EIGEN_SORT_ABS_ASC);
#ifdef AFFDEBUG
     {
          int i;

          for (i = 2; i < 5; i++)
          {
               double eval_i
                    = gsl_vector_get (eval, i);
               gsl_vector_view evec_i
                    = gsl_matrix_column (evec, i);

               printf("eigenvalue = %g\n", eval_i);
               printf("eigenvector = \n");
               gsl_vector_fprintf (stdout,
                                   &evec_i.vector, "%g");
          }
     }

     std::cout << "\n---- Colonne" << std::endl;
     for(int i = 0; i < nSommets; i++)
     {
          for(int j = 2; j < 5; j++)
               printf("%-7lf\t", gsl_matrix_get(evec, i, j));

          std::cout << std::endl;
     }
#endif

     // Ajout d'un repere orthonormé
     SceneVertex vertices_i = { { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } }; // Axe X
     SceneVertex vertices_j = { { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } }; // Axe X

     _vertices.push_back(vertices_i);
     _vertices.push_back(vertices_j);

     vertices_i = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } }; // Axe Y
     vertices_j = { { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } }; // Axe Y

     _vertices.push_back(vertices_i);
     _vertices.push_back(vertices_j);

     vertices_i = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }; // Axe Z
     vertices_j = { { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } }; // Axe Z

     _vertices.push_back(vertices_i);
     _vertices.push_back(vertices_j);

     for(int i = 0; i < 6; i++)
          _indices.push_back(i);
     
     // coordonnées de chaque sommets
     for(int i = 0; i < nSommets; i++)
     {
          //                            X      Y      Z          R      G      B
          SceneVertex vertices_i = { { 0.0f , 0.0f , 0.0f } , { 1.0f , 1.0f , 1.0f } };

          vertices_i.Position[0] = gsl_matrix_get(evec, i, 2);
          vertices_i.Position[1] = gsl_matrix_get(evec, i, 3);
          vertices_i.Position[2] = gsl_matrix_get(evec, i, 4);

          _vertices.push_back(vertices_i);
     }

     // Mise des valeurs dans les containeurs membres (vertices/indices)
     // Pour chaque arete
     for(int i = 0; i < nSommets; i++)
     {
          for(int j = i+1; j < nSommets; j++)
          {
               if(_matrice_lap->dat[i][j] != -1) // Si l'arete n'existe pas, on boucle
                    continue;

               // arete entre le noeud i et j
               _indices.push_back(i+6);
               _indices.push_back(j+6);
          }
     }

#ifdef AFFDEBUG
     for(unsigned int i = 0; i < _indices.size()-1; i+=2)
          printf("[%3d][%3d]\n", _indices.at(i), _indices.at(i+1));
#endif

     std::cout << "\nnombre d'indices : " << _indices.size()
               << "\nnombre de vertex : " << _vertices.size()
               << std::endl;

     _est_init = true;
}

/*
 * Création du graphe par liste d'adjacence
 */
void Graphe::creer_listes_adjacences()
{
     FILE* fichier = NULL;

     /* On ouvre le fichier en lecture seule ici, on sort si il y a echec à l'ouverture */
     if((fichier = fopen(_path_fichier, "r")) == NULL)
     {
          printf("ERREUR! Echec à l'ouverture du fichier");
          exit(EXIT_FAILURE);
     }

     /* Si on est ici alors le fichier est ouvert */
     _liste_adj = (liste_adjacence_t*) malloc(sizeof(liste_adjacence_t));

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

     // FIXME: utiliser std::array
     _liste_adj->dat = new Liste*[nSommets];

     int i;
     for(i = 0; i < nSommets; i++)
          _liste_adj->dat[i] = new Liste;

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
                    if(_liste_adj->dat[nb1]->rechercher(nb2) == NULL)
                         _liste_adj->dat[nb1]->inserer(creer_cellule(nb2));

                    // On ajoute la cellule contenant nb2 dans la liste numéro nb1
                    if(_liste_adj->dat[nb2]->rechercher(nb1) == NULL)
                         _liste_adj->dat[nb2]->inserer(creer_cellule(nb1));
               }
          }
     }
     else
          printf("Erreur structure fichier: %s", buff);

     fclose(fichier);

     _liste_adj->nbrS = nSommets;
     _liste_adj->nbrA = nArrete;
}

/*
 * Création du graphe par matrice d'adjacence
 */
void Graphe::creer_matrice_adjacences()
{
     FILE* fichier = NULL;

     /* On ouvre le fichier en lecture seule ici, on sort si il y a echec à l'ouverture */
     if((fichier = fopen(_path_fichier, "r")) == NULL)
     {
          printf("ERREUR! Echec à l'ouverture du fichier");
          exit(EXIT_FAILURE);
     }

     /* Si on est ici alors le fichier est ouvert */
     _matrice_adj = (matrice_adjacence_t*) malloc(sizeof(matrice_adjacence_t));

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
     _matrice_adj->dat = (int**) malloc(sizeof(int*)*nSommets);
     int i;
     for(i = 0; i < nSommets; i++)
          _matrice_adj->dat[i] = (int*) malloc(sizeof(int)*nSommets);

     /* Initialisation */
     int j;
     for(i = 0; i < nSommets; i++)
          for(j = 0; j < nSommets; j++)
               _matrice_adj->dat[i][j] = 0;

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
                    _matrice_adj->dat[nb1][nb2] = -1;
                    _matrice_adj->dat[nb2][nb1] = -1;
               }
          }
     }
     else
          printf("Erreur structure fichier: %s", buff);

     fclose(fichier);

     _matrice_adj->nbrS = nSommets;
     _matrice_adj->nbrA = nArrete;

     int* nbrdif = (int*) malloc(sizeof(int)*nSommets);

     for(i = 0; i < nSommets; i++)
     {
          nbrdif[i] = 0;

          for(j = 0; j < nSommets; j++)
          {
               if(_matrice_adj->dat[i][j] == -1)
                    nbrdif[i]++;
          }
     }

     for(i = 0; i < nSommets; i++)
          _matrice_adj->dat[i][i] = nbrdif[i];

     free(nbrdif);
}


/*
 * Création du graphe par matrice d'adjacence
 */
void Graphe::creer_matrice_laplace()
{
     //
     // Chargement fichier et compute laplacienne
     //
     FILE* fichier = NULL;

     /* On ouvre le fichier en lecture seule ici, on sort si il y a echec à l'ouverture */
     if((fichier = fopen(_path_fichier, "r")) == NULL)
     {
          printf("ERREUR! Echec à l'ouverture du fichier");
          exit(EXIT_FAILURE);
     }

     /* Si on est ici alors le fichier est ouvert */
     _matrice_lap = (matrice_laplace_t*) malloc(sizeof(matrice_laplace_t));

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
     _matrice_lap->dat = (int**) malloc(sizeof(int*)*nSommets);

     int i;
     for(i = 0; i < nSommets; i++)
          _matrice_lap->dat[i] = (int*) malloc(sizeof(int)*nSommets);

     /* Initialisation */
     int j;
     for(i = 0; i < nSommets; i++)
          for(j = 0; j < nSommets; j++)
               _matrice_lap->dat[i][j] = 0;

     int nb1, nb2;

     fscanf(fichier, "%s", buff);
     if(strcmp(buff, "debutDefAretes") == 0)
     {
          while(fgets(buff, 16, fichier) != 0 && strncmp(buff, "finDefAretes", 12) != 0)
          {
               if(strlen(buff) < 3) continue; // On peut pas etre en dessous de 3 caractere pour une ligne

               // Ici buff est composé de 2 nombres
               sscanf(buff, "%d %d", &nb1, &nb2);

               if(nb1 != nb2)
               {
                    _matrice_lap->dat[nb1][nb2] = -1;
                    _matrice_lap->dat[nb2][nb1] = -1;
               }
          }
     }
     else
          printf("Erreur structure fichier: %s", buff);

     fclose(fichier);
     
     _matrice_lap->nbrS = nSommets;
     _matrice_lap->nbrA = nArrete;
     
     // Calcul de la diagonale de la matrice laplacienne
     
     for(i = 0; i < nSommets; i++)
          for(j = 0; j < nSommets; j++)
          {
               // Si il y a une arete (i.e -> == -1), on augmente la diagonale
               if(_matrice_lap->dat[i][j] == -1)
                    _matrice_lap->dat[i][i]++;
          }
     
#ifdef AFFDEBUG
     afficher_matrice_laplace();
#endif

}

/*
 * Permet d'afficher la liste d'adjacence
 */
void Graphe::afficher_listes_adjacences(liste_adjacence_t* l)
{
     if(l == NULL) return;

     int i = 0;

     printf("\n================== "
            "\n// Liste adjacence"
            "\n------------------");

     for(i = 0; i < l->nbrS; i++)
     {
          printf("\n[%2d] -> ", i);
          l->dat[i]->afficher_liste();
     }
     printf("\n------------------");
}

/*
 * Permet d'afficher la matrice d'adjacence
 */
void Graphe::afficher_matrice_adjacences(matrice_adjacence_t* m)
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
 * Permet d'afficher la matrice laplace
 */
void Graphe::afficher_matrice_laplace()
{
     std::cout << "----" << std::endl;
     std::cout << "Matrice Laplace: " << std::endl;
     for(int i = 0; i < _S; i++)
     {
          for(int j = 0; j < _S; j++)
               printf("%2d\t", _matrice_lap->dat[i][j]);
          
          std::cout << std::endl;
     }
     
     std::cout << "----" << std::endl;
}

/*
 * Destruction d'un graphe
 */
Graphe::~Graphe()
{
     int i = 0;

     // Liberation memoire de la liste des sommets
     for(i = 0; i < _S; i++)
          free(_tabS[i]);

     free(_tabS);

     // Liberation memoire liste
     if(_liste_adj != NULL)
     {
          for(i = 0; i < _S; i++)
               delete _liste_adj->dat[i];

          delete[] _liste_adj->dat;         
          free(_liste_adj);
     }

     // Liberation memoire matrice
     if(_liste_adj != NULL)
     {
          for(i = 0; i < _S; i++)
               free(_matrice_adj->dat[i]);

          free(_matrice_adj->dat);
          free(_matrice_adj);
     }
}

/*
 * Parcours en largeur d'un graphe
 */
parcours_t* Graphe::parcours_largeur(sommet_t* s)
{
     parcours_t* p = (parcours_t*) malloc(sizeof(parcours_t));

     p->f = new File;

     // Initialisation
     int i = 0;
     sommet_t* u = NULL;
     sommet_t* v = NULL;

     for(i = 0; i < _S; i++)
     {
          u = _tabS[i];

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
     Liste* ladj = NULL;
     File* F = new File;

     F->enfiler(s);
     p->f->enfiler(s);

     while(! F->file_vide())
     {
          u = F->defiler();

          // recuperation de la liste d'adjacence
          ladj = _liste_adj->dat[u->val];

          c = ladj->_tete;
          while(c != NULL)
          {
               v = _tabS[c->val];
               if(v->couleur == BLANC)
               {
                    v->couleur = GRIS;
                    v->d = u->d + 1;
                    v->pere = u;
                    F->enfiler(v);
                    p->f->enfiler(v);
               }

               c = c->succ;
          }

          u->couleur = NOIR;
     }

     delete F;

     return p;
}

void Graphe::afficher_chemin(sommet_t* i, sommet_t* j)
{
     if(i == NULL || j == NULL) return;

     if(i->val == j->val)
          printf("\t[%d]", i->val);
     else if(j->pere == NULL)
          printf("\t no(%d;%d)", i->val, j->val);
     else
     {
          afficher_chemin(i, j->pere);
          printf("\t[%d]", j->val);
     }
}

void Graphe::afficher_parcours_profondeur()
{
     int i;
     for(i = 0; i < _S; i++)
     {
          printf("\n[%2d] {D: %2d ; F: %2d}", i, _tabS[i]->d, _tabS[i]->f);
     }
}

parcours_t* Graphe::parcours_profondeur()
{
     parcours_t* p = (parcours_t*) malloc(sizeof(parcours_t));
     p->f = new File;

     sommet_t* u = NULL;
     int date = 0;

     int i;
     for(i = 0; i < _S; i++)
     {
          u = _tabS[i];
          u->couleur = BLANC;
          u->pere = NULL;
     }

     date = 0;
     for(i = 0; i < _S; i++)
     {
          u = _tabS[i];
          if(u->couleur == BLANC)
               DFS_visiter_noeud(u, &date, p);
     }

     return p;
}

void Graphe::DFS_visiter_noeud(sommet_t* u, int* time, parcours_t* p)
{
     p->f->enfiler(u);

     *time += 1;

     u->d = *time;
     u->couleur = GRIS;

     sommet_t* v = NULL;
     Liste* ladj = NULL;
     cellule_t* c = NULL;
     // recuperation de la liste d'adjacence
     ladj = _liste_adj->dat[u->val];

     c = ladj->_tete;
     while(c != NULL)
     {
          v = _tabS[c->val];
          if(v->couleur == BLANC)
          {
               v->pere = u;
               DFS_visiter_noeud(v, time, p);
          }

          c = c->succ;
     }

     u->couleur = NOIR;
     *time += 1;
     u->f = *time;
}
