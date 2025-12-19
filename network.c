#include "network.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Segment* parse_csv_segment(const char *line) {
//     // Parsing robuste d'une ligne CSV (séparateur ;)
//     Segment *seg = calloc(1, sizeof(Segment));
//     char *tmp = strdup(line);
//     char *saveptr = NULL;
//     char *token = strtok_r(tmp, ";", &saveptr);
//     int col = 0;
//     
//     while (token != NULL) {
//         if (col == 0) {
//             // plant_id
//             if (strcmp(token, "-") != 0) {
//                 seg->plant_id = strdup(token);
//             } else {
//                 seg->plant_id = NULL;
//             }
//         } else if (col == 1) {
//             // upstream_id
//             if (strcmp(token, "-") != 0) {
//                 seg->upstream_id = strdup(token);
//             } else {
//                 seg->upstream_id = NULL;
//             }
//         } else if (col == 2) {
//             // downstream_id
//             if (strcmp(token, "-") != 0) {
//                 seg->downstream_id = strdup(token);
//             } else {
//                 seg->downstream_id = NULL;
//             }
//         } else if (col == 3) {
//             // volume
//             if (strcmp(token, "-") != 0 && *token != '\0') {
//                 seg->volume = atof(token);
//             } else {
//                 seg->volume = 0.0;
//             }
//         } else if (col == 4) {
//             // leakage
//             if (strcmp(token, "-") != 0 && *token != '\0') {
//                 seg->leakage = atof(token);
//             } else {
//                 seg->leakage = 0.0;
//             }
//         }
//         
//         token = strtok_r(NULL, ";", &saveptr);
//         col = col + 1;
//     }
//     
//     free(tmp);
//     return seg;
// }
//
//
// void free_segment(Segment *seg) {
//     if (seg == NULL) {
//         return;
//     }
//     
//     free(seg->plant_id);
//     free(seg->upstream_id);
//     free(seg->downstream_id);
//     free(seg);
// }
