#include "utils.h"
#include <stdlib.h>

// Comparaison de deux chaînes
int cmp_str(const char *a, const char *b) {
    if (!a && !b) return 0;
    if (!a) return -1;
    if (!b) return 1;
    return strcmp(a, b);
}

// Libération sécurisée
void safe_free(void *ptr) {
    if (ptr) {
        free(ptr);
    }
}