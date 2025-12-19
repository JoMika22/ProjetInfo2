#ifndef NETWORK_H
#define NETWORK_H

#include "avl.h"

// Structures pour tronçon, usine, source, n-aire, etc.

typedef struct Segment {
    char *plant_id;
    char *upstream_id;
    char *downstream_id;
    double volume;
    double leakage;
    struct Segment *next;
} Segment;

typedef struct Plant {
    char *id;
    double max_capacity;
    AVLNode *sources; 
} Plant;

typedef struct NNode {
    char *id;
    double volume;
    double leakage;
    struct NNode **children;
    int child_count;
} NNode;


#endif // NETWORK_H
