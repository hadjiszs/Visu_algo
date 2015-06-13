#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "file.hpp"

#define MAXIMUM_FILE 200000

File::File()
{
     _tab = (sommet_t**) malloc(sizeof(sommet_t*)*MAXIMUM_FILE);
     _tete = 0;
     _queue = 0;
}

File::~File()
{
     if(_tab != NULL)
	  free(_tab);
}

int File::file_vide()
{
     return _tete == _queue;
}

void File::enfiler(sommet_t* s)
{
     if(s == NULL ) return;

     _tab[_queue] = s;

     if(_queue == MAXIMUM_FILE)
	  _queue = 1;
     else
	  _queue++;
}

sommet_t* File::defiler()
{
     if(file_vide() == 1) return NULL;
     
     sommet_t* res = _tab[_tete];
     
     if(_tete == MAXIMUM_FILE)
	  _tete = 1;
     else
	  _tete++;
     
     return res;
}

File::File(File* f)
{
     memcpy(_tab, f->_tab, sizeof(sommet_t*)*MAXIMUM_FILE);

     _tete = f->_tete;
     _queue = f->_queue;
}

void File::afficher_file()
{
     if(file_vide() == 1) printf("\n_file vide");

     File* copie = new File(this);
     
     printf("\nfile: ");
     
     sommet_t* tmp = NULL;
     while(! copie->file_vide())
     {
	  tmp = copie->defiler();
	  printf("\t%2d", tmp->val);
     }
     
     delete copie;
}
