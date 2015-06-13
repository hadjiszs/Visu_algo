#ifndef _GRAPHE_HPP_
#define _GRAPHE_HPP_
//#include "scene.hpp"
namespace LuaGraphe
{

struct graphe
{
    int** m;
    int n;
    int o;
    int v;
    int c;
    graphe(std::string s);
    ~graphe();
    void creerMatriceAdjacences(std::string s);
    void afficherMatriceAdjacences();
    void setMatrice(int i, int j, int v);
    int getMatrice(int i, int j);
/*
    float** barycentric_drawing();
    friend float norme(float* sv);
    friend void normalize(float* sv);
    friend float* vecteur(float* a, float* b);
    friend float fa(float x, float k);
    friend float fr(float x, float k);
    friend int cool(int t);*/
    //void exporter_aretes(std::vector<GLuint>& v);
    //void exporter_sommet(std::vector<SceneVertex>& v, float** pos, float* color);
};

void afficherGraphe(std::string f);
extern void addToState(lua_State* L);

}


#endif
