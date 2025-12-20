#ifndef PARSER_H
#define PARSER_H

#include "network.h"
#include "utils.h"

// Dans le cadre de notre projet le parsing est l'action de lire et découper chaque ligne du fichier CSV selon le caractère ';' 
// pour extraire les identifiants et les volumes afin de les transformer en données exploitables par notre programme C.

// Allouer un nouveau segment
Segment* new_segment();

// Parser une ligne CSV
Segment* parse_csv_segment(const char *line);

// Libérer un segment
void free_segment(Segment *seg);

// Parser un fichier CSV complet
int parse_csv_file(const char *filename, int (*callback)(Segment*, void*), void *userdata);

#endif // PARSER_H
