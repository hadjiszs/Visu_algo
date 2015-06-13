#ifndef _ENSEMBLE_H_
#define _ENSEMBLE_H_
#include "liste.h"

struct ensemble
{
    liste* l;
};

typedef struct ensemble ensemble;

ensemble* creerEnsemble(sommet* x);
sommet* trouverEnsemble(sommet* x);
void unionEnsemble(sommet* x, sommet* y);
void afficherEnsemble(ensemble* e);
void detruireEnsemble(ensemble* e);



#endif // _ENSEMBLE_H_
