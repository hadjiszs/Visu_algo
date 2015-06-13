#ifndef _FILE_H_
#define _FILE_H_
#include "liste.h"

struct file
{
    liste* l;
};

typedef struct file file;
file* creerFile();
void detruireFile(file** f);
int fileVide(file* f);
void enfiler(file* f, sommet* s);
sommet* defiler(file* f);


#endif // _FILE_H_
