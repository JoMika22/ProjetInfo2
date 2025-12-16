#ifndef CSV_READER_H
#define CSV_READER_H

#include <stdio.h>
#include "avl.h"

FILE* abrirFichierCSV(char* cheminFichier);
void fermerFichierCSV(FILE* fichier);
int lireLigneSuivante(FILE* fichier, Station* s);
int extraireID(char* chaine);

#endif
