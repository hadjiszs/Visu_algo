#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <string.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ctime>
#define inf INT_MAX
using namespace luabridge;
namespace LuaGraphe
{

graphe::graphe(std::string f)
{
    creerMatriceAdjacences(f);
}

graphe::~graphe()
{
    if(m != NULL)
    {
        for(int i = 0; i < n; i++)
        {
            if(m[i] != NULL)
                delete m[i];
        }
        delete m;
    }
}

void graphe::creerMatriceAdjacences(std::string nom)
{
    char* f = const_cast<char*>(nom.c_str());
    FILE* fichier = fopen(f, "r");
    int temp1;
    int temp2;
    o = 0;
    v = 0;
    c = 0;
    char* s = (char*)malloc(sizeof(char)*50);
    fscanf(fichier,"%s",s);
    fscanf(fichier,"%d",&n);
    fscanf(fichier,"%s",s);
    fscanf(fichier,"%d",&o);
    fscanf(fichier,"%s",s);
    fscanf(fichier,"%d",&v);
    fscanf(fichier,"%s",s);
    fscanf(fichier,"%d",&c);
    m = (int**)malloc(sizeof(int*)*(n));
    for(int i=0; i<n; i++)
    {
        m[i] = (int*)malloc(sizeof(int)*(n));
        for(int j=0; j<n; j++)
        {
            m[i][j]=0;
        }
    }
    int temp3 = 0;
    fscanf(fichier,"%s",s);
    while(fscanf(fichier,"%s",s)!=-1)
    {
        if(strcmp(s,"finDefAretes")!=0)
        {
            temp1 = atoi(s);
            fscanf(fichier,"%d",&temp2);
            if(v)
            {
                fscanf(fichier,"%d",&temp3);
            }
            if(o)
            {
                if(v)
                {
                    m[temp1][temp2] = temp3;
                }
                else
                {
                    m[temp1][temp2] = 1;
                }
            }
            else
            {
                if(v)
                {
                    m[temp1][temp2] = temp3;
                    m[temp2][temp1] = temp3;
                }
                else
                {
                    m[temp1][temp2] = 1;
                    m[temp2][temp1] = 1;
                }

            }
        }
    }
    fclose(fichier);
}

/*
 * Permet d'afficher la matrice d'adjacence
 */
void graphe::afficherMatriceAdjacences()
{
     int nbr_sep = 8 + 4*n;
     char* sep = (char*) malloc(sizeof(char)*(nbr_sep + 1));

     for(int k = 0; k < nbr_sep; k++) sep[k] = '-';

     sep[nbr_sep] = '\0';

     printf("\n==================== "
        "\n// Matrice adjacence"
        "\n%s"
        "\n       |", sep);

     for(int i = 0; i < n; i++)
      printf(" %-2d|", i);

     for(int i = 0; i < n; i++)
     {
      printf("\n%s"
        "\n[%2d] - |", sep, i);

      for(int j = 0; j < n; j++)
            printf("%2d|", m[i][j]);

     }
     printf("\n%s", sep);

     delete sep;
}

void graphe::setMatrice(int i, int j, int v)
{
	m[i][j] = v;
}

int graphe::getMatrice(int i, int j)
{
	int v = m[i][j];
	return v;
}

/*
float fa(float x, float k)
{
    return x*x/k;
}

float fr(float x, float k)
{
    return k*k/x;
}

int cool(int t)
{
    return (int)((float)t*0.95);
}

float norme(float* v)
{
    return std::sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

void normalize(float* v)
{
    int d = norme(v);
    v[0] = v[0] / d;
    v[1] = v[1] / d;
    v[2] = v[2] / d;
}

float* vecteur(float* a, float* b)
{
    float* c = new float[3];
    c[0] = b[0] - a[0];
    c[1] = b[1] - a[1];
    c[2] = b[2] - a[2];
    return c;
}

float** graphe::barycentric_drawing()
{
    std::srand(std::time(0));
    float** pos = new float*[n];
    float** d = new float*[n];
    for(int i = 0; i < n; i++)
    {
        pos[i] = new float[3];
        pos[i][0] = std::rand()%(n);
        pos[i][1] = std::rand()%(n);
        pos[i][2] = std::rand()%(n);
        d[i] = new float[3];
        d[i][0] = 0;
        d[i][1] = 0;
        d[i][2] = 0;
    }
    int volume = n*n*n;
    float k = std::pow(volume/n,1.0/3.0);
    printf("\n k %f n %d\n",k,n);
    int t = (n+n+n)/3;
    float* delta;
    float force = 0;
    float norme_d = 0;

    while(t > 1)
    {
        for(int v = 0; v < n; v++)
        {
            d[v][0] = 0;
            d[v][1] = 0;
            d[v][2] = 0;
            for(int u = 0; u < n; u++)
            {
                delta = vecteur(pos[u],pos[v]);
                norme_d = norme(delta);
                if(u != v)
                {
                    if(norme_d != 0)
                    {
                        force = fr(norme_d,k);
                        d[v][0] += force * delta[0]/norme_d;
                        d[v][1] += force * delta[1]/norme_d;
                        d[v][2] += force * delta[2]/norme_d;
                    }
                }
            }
        }

        for(int v = 0; v < n; v++)
        {
            for(int u = 0; u < n; u++)
            {
                if(this->m[v][u] == 1)
                {
                    delete delta;
                    delta = vecteur(pos[u],pos[v]);
                    norme_d = norme(delta);
                    if(norme_d != 0)
                    {
                        force = fa(norme_d,k);
                        d[v][0] -= force*delta[0]/norme_d;
                        d[v][1] -= force*delta[1]/norme_d;
                        d[v][2] -= force*delta[2]/norme_d;
                        d[u][0] += force*delta[0]/norme_d;
                        d[u][1] += force*delta[1]/norme_d;
                        d[u][2] += force*delta[2]/norme_d;
                    }
                }
            }
        }

        for(int v = 0; v < n; v++)
        {
            norme_d = norme(d[v]);
            pos[v][0] += pos[v][0] + std::min(norme_d,(float)t)*d[v][0]/norme_d;
            pos[v][1] += pos[v][1] + std::min(norme_d,(float)t)*d[v][1]/norme_d;
            pos[v][2] += pos[v][2] + std::min(norme_d,(float)t)*d[v][2]/norme_d;
            normalize(pos[v]);
            pos[v][0] *= n;
            pos[v][1] *= n;
            pos[v][2] *= n;
        }
        t = cool(t);
    }
    for(int v = 0; v < n; v++)
    {
        normalize(pos[v]);
        printf("\n %f %f %f",pos[v][0],pos[v][1],pos[v][2]);
    }
    return pos;
}*/

/*void graphe::exporter_aretes(std::vector<GLuint>& v)
{
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(m[i][j] == 1)
            {
                v.push_back((GLuint)i);
                v.push_back((GLuint)j);
            }
        }
    }
}

void graphe::exporter_sommet(std::vector<SceneVertex>& v, float** pos, float* color)
{
    SceneVertex temp;
    for(int i = 0; i < n; i++)
    {
        temp.Position[0] = pos[i][0];
        temp.Position[1] = pos[i][1];
        temp.Position[2] = pos[i][2];
        temp.Color[0] = color[0];
        temp.Color[1] = color[1];
        temp.Color[2] = color[2];
        v.push_back(temp);
    }
}*/

void afficherGraphe(std::string f)
{
	graphe g(f.c_str());
	g.afficherMatriceAdjacences();
}

void addToState(lua_State* L)
{
	getGlobalNamespace(L)
		.addFunction("afficherGraphe",&afficherGraphe)
		.beginClass <graphe> ("graphe")
			.addData("m",&graphe::m,false)		
			.addData("n",&graphe::n)
			.addData("o",&graphe::o)
			.addData("v",&graphe::v)
			.addData("c",&graphe::c)
			.addConstructor <void (*) (std::string)>()
			.addFunction("creerMatrice",&graphe::creerMatriceAdjacences)
			.addFunction("afficherMatrice",&graphe::afficherMatriceAdjacences)
			.addFunction("set",&graphe::setMatrice)
			.addFunction("get",&graphe::getMatrice)
		.endClass();
}	

}

