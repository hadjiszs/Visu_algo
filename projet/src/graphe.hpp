/**
 * \file graphe.hpp
 * \brief Gère la structure de donnée de graphe
 */
#ifndef _GRAPHE_
#define _GRAPHE_

#include "liste.hpp"
#include "file.hpp"

#include "structure.hpp"

#include "textbox.hpp"

typedef struct {
     Liste** dat;
     int nbrA;
     int nbrS;
} liste_adjacence_t;

typedef struct {
     int** dat;
     int nbrA;
     int nbrS;
} matrice_laplace_t;

typedef struct {
     int** dat;
     int nbrA;
     int nbrS;
} matrice_adjacence_t;

typedef struct {
     File* f;
} parcours_t;

/**
 * \class Graphe
 * \brief Classe gerant la structure de donnée de graphe
 */
class Graphe : public Structure {
private:
     int                        _A; // Nombre d'arrete
     int                        _S; // Nombre noeuds

     liste_adjacence_t*         _liste_adj;
     matrice_adjacence_t*       _matrice_adj;
     matrice_laplace_t*         _matrice_lap;
     
     sommet_t**                 _tabS;

public:
     /**
      * \brief Constructeur de la structure de graphe
      * \details Structure de données permettant de manipuler des graphes et des algos de graphe. Le graphe ne sera pas charger, il est juste créer, pour le charger il faut appelé explicitement la fonction charger()
      * \param[in] path         chemin vers le fichier contenant le graphe
      * \param[in] textbox      widget permettant l'affichage de texte
      */
     Graphe(std::string path, TextBox* textbox);

     /**
      * \brief Destructeur
      */
     ~Graphe();

     /**
      * \brief Chargement du graphe
      * \details Chargement du graphe de maniere explicite à partir de l'appel de cette fonction
      */
     void charger();

     /**
      * \brief Calcul du scale
      * \details Calcul du scale pour le dessin des mesh representant les noeuds
      */
     void compute_scale();
     
     /**
      * \brief Creer liste d'adjacence
      */
     void creer_listes_adjacences();

     /**
      * \brief Creer matrice d'adjacence
      */
     void creer_matrice_adjacences();

     /**
      * \brief Creer matrice laplacienne
      */
     void creer_matrice_laplace();

     /**
      * \brief Parcours en largeur
      * \details Algorithme BFS, parcours en largeur sur le graphe courant
      * \param[in]  s sommet de depart pour le parcours en largeur
      * \param[out] parcours en largeur resultant
      */
     parcours_t* parcours_largeur(sommet_t* s);

     /**
      * \brief Parcours en profondeur
      * \details Algorithme DFS, parcours en profondeur sur le graphe courant
      * \param[out] parcours en largeur resultant
      */
     parcours_t* parcours_profondeur();

     /**
      * \brief Affichage du parcours en profondeur
      * \details Affiche le parcours en profondeur resultant, effectuer l'appel à cette fonction après avoir fait un parcours en profndeur
      */     
     void afficher_parcours_profondeur();
     
     /**
      * \brief Fonction interne à l'algorithme DFS
      * \param[in] u sommet à visiter
      * \param[in] time date à laquelle on visite le noeud
      * \param[in] p parcours dans lequel est integré la visite du noeud
      */          
     void DFS_visiter_noeud(sommet_t* u, int* time, parcours_t* p);
     
     /**
      * \brief Affichage liste d'adjacences
      * \details TODO: utiliser operator<<
      */
     void afficher_listes_adjacences(liste_adjacence_t* l);
     
     /**
      * \brief Affichage matrice d'adjacences
      * \details TODO: utiliser operator<<
      */     
     void afficher_matrice_adjacences(matrice_adjacence_t* l);

     /**
      * \brief Affichage matrice laplace
      * \details TODO: utiliser operator<<
      */     
     void afficher_matrice_laplace();

     /**
      * \brief Affichage chemin
      * \details affiche le chemin après un BFS, permet de voir le plus court chemin entre deux noeud passé en parametre
      * \param[in] i sommet de depart
      * \param[in] j sommet d'arrivé
      */     
     void afficher_chemin(sommet_t* i, sommet_t* j);
};

#endif
