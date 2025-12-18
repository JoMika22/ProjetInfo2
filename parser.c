#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Allouer et remplir un segment
Segment* new_segment() {
    Segment *seg = calloc(1, sizeof(Segment));
    if (!seg) return NULL;
    return seg;
}

// Parser une ligne CSV
Segment* parse_csv_segment(const char *line) {
    if (!line) return NULL;
    
    Segment *seg = new_segment();
    if (!seg) return NULL;
    
    char *tmp = strdup(line);
    if (!tmp) {
        free(seg);
        return NULL;
    }
    
    char *saveptr = NULL;
    char *token = strtok_r(tmp, ";", &saveptr);
    int col = 0;
    
    while (token && col < 5) {
        // Trim whitespace
        while (*token == ' ') token++;
        char *end = token + strlen(token) - 1;
        while (end >= token && *end == ' ') *(end--) = '\0';
        
        switch (col) {
            case 0: // plant_id
                if (strcmp(token, "-") != 0) {
                    seg->plant_id = strdup(token);
                }
                break;
            case 1: // upstream_id
                if (strcmp(token, "-") != 0) {
                    seg->upstream_id = strdup(token);
                }
                break;
            case 2: // downstream_id
                if (strcmp(token, "-") != 0) {
                    seg->downstream_id = strdup(token);
                }
                break;
            case 3: // volume
                if (strcmp(token, "-") != 0 && *token) {
                    seg->volume = atof(token);
                }
                break;
            case 4: // leakage
                if (strcmp(token, "-") != 0 && *token) {
                    seg->leakage = atof(token);
                }
                break;
        }
        
        token = strtok_r(NULL, ";", &saveptr);
        col++;
    }
    
    free(tmp);
    return seg;
}

// Libérer un segment
void free_segment(Segment *seg) {
    if (!seg) return;
    safe_free(seg->plant_id);
    safe_free(seg->upstream_id);
    safe_free(seg->downstream_id);
    safe_free(seg);
}

// Parser un fichier CSV complet
int parse_csv_file(const char *filename, int (*callback)(Segment*, void*), void *userdata) {
    if (!filename || !callback) return 1;
    
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Erreur : impossible d'ouvrir %s\n", filename);
        return 2;
    }
    
    char line[2048];
    int line_num = 0;
    
    // Lire et ignorer l'en-tête
    if (fgets(line, sizeof(line), f)) {
        line_num++;
    }
    
    // Traiter chaque ligne
    while (fgets(line, sizeof(line), f)) {
        line_num++;
        
        // Supprimer le \n
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        // Ignorer les lignes vides
        if (strlen(line) == 0) continue;
        
        Segment *seg = parse_csv_segment(line);
        if (seg) {
            int res = callback(seg, userdata);
            free_segment(seg);
            if (res != 0) {
                fclose(f);
                return res;
            }
        }
    }
    
    fclose(f);
    return 0;
}