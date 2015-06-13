#ifndef _FILE_
#define _FILE_

typedef enum {BLANC, NOIR, GRIS} couleur_t;

typedef struct sommet {
     couleur_t couleur;
     int f;
     int d;   
     int val;
     struct sommet* pere;
} sommet_t;

class File {     
public:
     sommet_t**         _tab;
     int                _tete;
     int                _queue;
     
     File();
     File(File*);
     ~File();
     
     int file_vide();
     void enfiler(sommet_t*);
     sommet_t* defiler();
     void afficher_file();
};

#endif
