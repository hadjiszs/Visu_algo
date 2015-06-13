#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "file.h"

int main(void)
{
	file_t* f = creer_file();
	
	sommet_t* s1 = (sommet_t*) malloc(sizeof(sommet_t));
	sommet_t* s2 = (sommet_t*) malloc(sizeof(sommet_t));
	sommet_t* s3 = (sommet_t*) malloc(sizeof(sommet_t));
	sommet_t* s4 = (sommet_t*) malloc(sizeof(sommet_t));
	sommet_t* s5 = (sommet_t*) malloc(sizeof(sommet_t));
					
	s1->val = 110;
	s2->val = 75;
	s3->val = 19;
	s4->val = 62;
	s5->val = 31;
	
	enfiler(f, s1);
	enfiler(f, s2);
	enfiler(f, s3);
	enfiler(f, s4);
	enfiler(f, s5);

	afficher_file(f);
	
	s1 = defiler(f);
	if(s1 != NULL)
		printf("\npop: %d", s1->val);
	else
		printf("\npop vide!!");
		
	s1 = defiler(f);
	if(s1 != NULL)
		printf("\npop: %d", s1->val);
	else
		printf("\npop vide!!");
		
	s1 = defiler(f);
	if(s1 != NULL)
		printf("\npop: %d", s1->val);
	else
		printf("\npop vide!!");
		
	s1 = defiler(f);
	if(s1 != NULL)
		printf("\npop: %d", s1->val);
	else
		printf("\npop vide!!");
		
	s1 = defiler(f);
	if(s1 != NULL)
		printf("\npop: %d", s1->val);
	else
		printf("\npop vide!!");
		
	s1 = defiler(f);
	if(s1 != NULL)
		printf("\npop: %d", s1->val);
	else
		printf("\npop vide!!");
		
	s1 = defiler(f);
	if(s1 != NULL)
		printf("\npop: %d", s1->val);
	else
		printf("\npop vide!!");	
				
	return 0;
}