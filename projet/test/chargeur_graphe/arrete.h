#ifndef _ARRETE_H_
#define _ARRETE_H_

struct arrete
{
    struct arrete* suiv;
    struct arrete* prec;
    int s1;
    int s2;
    int p;
};

typedef struct arrete arrete;

arrete* creerArrete(int a, int b, int c);
void detruireArrete(arrete** a);
void triArretes(arrete** a, int n);
void afficherArrete(arrete* a);
void afficherArretes(arrete** ar, int n);
#endif // _ARRETE_H_
