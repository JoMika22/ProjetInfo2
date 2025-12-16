#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv_reader.h"

FILE* abrirFichierCSV(char* cheminFichier) {
    FILE* fichier = fopen(cheminFichier, "r");
    if (fichier == NULL) {
        fprintf("Erreur lors de l'ouverture du fichier CSV");
        exit(1);
    }
    return fichier;
}
void fermerFichierCSV(FILE* fichier) {
    if (fichier != NULL) {
        fclose(fichier);
    }
}
int extraireID(char* chaine) {
    int i = 0;
    while (chaine[i] != '\0') {
        if (chaine[i] >= '0' && chaine[i] <= '9') {
            return atoi(&chaine[i]);
        }
        i++;
    }
    return 0; 
}

int lireLigneSuivante(FILE* fichier, Station* s) {
    char ligne[1024];
    if (fgets(ligne, 1024, fichier) == NULL) {
        return 0;
    }

    strcpy(ligneTemp, ligne);
    char* col1 = strtok(ligneTemp, ";"); 
    char* col2 = strtok(NULL, ";");     
    char* col3 = strtok(NULL, ";");      
    char* col4 = strtok(NULL, ";");     

   
    if (col2 == NULL || col4 == NULL) {
        return -1;
    }
    s->capacite = 0;
    s->consommation = 0;

    // CAS 1 : Définition d'usine (col3 est "-")
    if (col3 != NULL && strcmp(col3, "-") == 0) {
        s->id = extraireID(col2);
        s->capacite = atol(col4); // atol = ascii to long
        return 1;
    }

    /
    else if (col3 != NULL && strcmp(col3, "-") != 0) {
        if (strstr(col3, "Station") != NULL) {
            s->id = extraireID(col3);
            s->consommation = atol(col4);
            return 1;
        }
        if (strstr(col3, "Facility") != NULL) {
            s->id = extraireID(col3);
            s->consommation = atol(col4);
            return 1;
        }
        if (strstr(col3, "Plant") != NULL) {
            s->id = extraireID(col3);
            s->consommation = atol(col4);
            return 1;
        }
    }

    return -1;
}
