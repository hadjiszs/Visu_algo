#ifndef _GRAPHE_HPP_
#define _GRAPHE_HPP_
#include "scene.hpp"

class graphe
{
private:

    int** m;
    int n;
    int o;
    int v;
    int c;

public:
    graphe(char* f);
    ~graphe();
    void creerMatriceAdjacences(char* f);
    void afficherMatriceAdjacences();
    float** barycentric_drawing();
    friend float norme(float* sv);
    friend void normalize(float* sv);
    friend float* vecteur(float* a, float* b);
    friend float fa(float x, float k);
    friend float fr(float x, float k);
    friend int cool(int t);
    void exporter_aretes(std::vector<GLuint>& v);
    void exporter_sommet(std::vector<SceneVertex>& v, float** pos, float* color);
};




#endif
