#include "network.h"
#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "network.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Segment* parse_csv_segment(const char *line) {
    // Parsing robuste d'une ligne CSV (séparateur ;)
    Segment *seg = calloc(1, sizeof(Segment));
    char *tmp = strdup(line);
    char *saveptr = NULL;
    char *token = strtok_r(tmp, ";", &saveptr);
    int col = 0;
    while (token) {
        switch (col) {
            case 0: seg->plant_id = strcmp(token, "-") ? strdup(token) : NULL; break;
            case 1: seg->upstream_id = strcmp(token, "-") ? strdup(token) : NULL; break;
            case 2: seg->downstream_id = strcmp(token, "-") ? strdup(token) : NULL; break;
            case 3: seg->volume = (strcmp(token, "-") && *token) ? atof(token) : 0.0; break;
            case 4: seg->leakage = (strcmp(token, "-") && *token) ? atof(token) : 0.0; break;
        }
        token = strtok_r(NULL, ";", &saveptr);
        col++;
    }
    free(tmp);
    return seg;
}

void free_segment(Segment *seg) {
    if (!seg) return;
    free(seg->plant_id);
    free(seg->upstream_id);
    free(seg->downstream_id);
    free(seg);
}
