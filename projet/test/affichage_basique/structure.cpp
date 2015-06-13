/**
 * \file structure.cpp
 * \brief Implementation de structure.hpp
 */
#include "graphe.hpp"
#include "structure.hpp"

//#define DEBUG

Structure::Structure(std::string nom_fichier)
{
     _est_init     = false;
     _path_fichier = nom_fichier.c_str();
}

void Structure::compute_coordonnes()
{

}

std::vector<SceneVertex>* Structure::get_vertices()
{
     return &_vertices;
}

std::vector<GLuint>* Structure::get_indices()
{
     return &_indices;
}

bool Structure::est_init()
{
     return _est_init;
}

Structure::~Structure()
{

}
