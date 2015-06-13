#ifndef _LISTE_
#define _LISTE_

#include <string.h>
#include "cellule.hpp"

class Liste {
public:
     cellule_t* _tete;
     
     Liste();
     ~Liste();
     
     void       inserer(cellule_t*);
     cellule_t* rechercher(int);
     void       supprimer(cellule_t*);
     int        compter_liste();
     void       afficher_liste();
};

#endif
