/**
 * \file structure.hpp
 * \brief Gère les structures de données
 */
#ifndef _STRUCTURE_
#define _STRUCTURE_

#include <GL/glcorearb.h>
#include <iostream>
#include <vector>

#include <cstring>

#include <gsl/gsl_math.h>
#include <gsl/gsl_eigen.h>

#include "daeloader/colladainterface.h"

#include "textbox.hpp"

// Model de donnée typée (model 3D associé à chaque état)
namespace Etat
{
     enum class Noeud { SIMPLE , COURANT , VISITED , SELECT , CHOIX };
//     enum class Arbre { SIMPLE , ACTIF , FEUILLE , FILS , PERE , ONCLE }; // Pour eventuel visualisation d'algo sur arbre
}

/**
 * \struct SceneVertex
 * \brief structure d'un vertex incluant sa position et couleur
 */
struct SceneVertex
{
     // Dimension pour la position des vertex
     // ici 3 car en 3D
     static const int PositionSize = 3;

     // Dimension pour la couleur
     // ici 3 car RGB sans canal alpha
     static const int NormalSize = 3;

     float Position[PositionSize];
     float Normal[NormalSize];
};

/**
 * \class Structure
 * \brief Classe gerant les structures de données chargées dans l'IHM
 */
class Structure { // sera virtuelle ou sous forme de template
protected:
     // Coordonnées des sommets de la structure
     const char*                _path_fichier;
     std::vector<SceneVertex>   _vertices; // FIXME: to const
     bool                       _est_init;

     // ordre des indices des coordonnées de la structure
     // 0, 1, 2, 0, 4, 5
     // -> génerera l'arete des coordonnées d'indice 0 et 1
     // -> puis 2 et 0, puis 4 et 5
     std::vector<GLuint>        _indices;

     // On associe à chaque coordonnées (indice de coord en clef) son etat
     std::vector<Etat::Noeud>   _vertex_to_etat;

     // Scale du model (correspond à la plus petite norme entre deux sommets divisé par 2)
     double                     _scale;

     TextBox*                   _text_box;
     
public:
     /**
      * \brief Constructeur de la structure de donnée
      * \details Structure de données qui sera chargé de maniere à etre interfaçable dans l'IHM
      * \param[in] nom_fichier  nom de fichier à charger pour la structure
      * \param[in] textbox      widget permettant l'affichage de texte
      */
     Structure(std::string nom_fichier, TextBox* textbox);
     
     /**
      * \brief Destructeur virtuel
      */
     virtual ~Structure();

     /**
      * \brief Calcul du placement
      * \details Calculer les coordonnées de placement pour l'affichage de la structure de donnée courante
      */
     void compute_coordonnes();

     /**
      * \brief Chargement de la structure de donnée
      * \details Chargement de la structure de donnée à partir du fichier, appelé lors de la creation de l'instance courante
      * \param[in] nom_fichier  nom de fichier qui va etre chargé pour la structure
      */
     virtual void charger() = 0;

     /**
      * \brief Est initialisé
      * \details permet de savoir si la structure courante est bien initialisé et prete à etre chargé dans la scene opengl
      * \param[out] retourne un booleen specifiant si la structure courante est bien initialisé
      */
     bool est_init();
     
     //
     // Getters
     //
     std::vector<GLuint>*       get_indices();
     std::vector<SceneVertex>*  get_vertices();
     std::vector<Etat::Noeud>*  get_vertex_to_etat();
     double                     get_scale();
};

#endif
