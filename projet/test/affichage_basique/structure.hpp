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
     static const int ColorSize = 3;

     float Position[PositionSize];
     float Color[ColorSize];
};

/**
 * \class Structure
 * \brief Classe gerant les structures de données chargées dans l'IHM
 */
class Structure { // sera virtuelle ou sous forme de template
protected:
     // Coordonnées des sommets de la structure
     const char*                _path_fichier;
     std::vector<SceneVertex>   _vertices;
     bool                       _est_init;

     // ordre des indices des coordonnées de la structure
     // 0, 1, 2, 0, 4, 5
     // -> génerera l'arete des coordonnées d'indice 0 et 1
     // -> puis 2 et 0, puis 4 et 5
     std::vector<GLuint>        _indices;

public:
     /**
      * \brief Constructeur de la structure de donnée
      * \details Structure de données qui sera chargé de maniere à etre interfaçable dans l'IHM
      * \param[in] nom_fichier  nom de fichier à charger pour la structure
      */
     Structure(std::string nom_fichier);

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
     std::vector<SceneVertex>* get_vertices();
     std::vector<GLuint>* get_indices();
};

#endif
