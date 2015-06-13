#ifndef _FILE_
#define _FILE_

#include "type.h"

file_t* creer_file();
void detruire_file(file_t*);
int file_vide(file_t*);
void enfiler(file_t*, sommet_t*);
sommet_t* defiler(file_t*);
void afficher_file(file_t*);

#endif
