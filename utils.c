#include "utils.h"
#include <stdlib.h>
#include <string.h>

// Comparaison de deux chaînes
int cmp_str(const char *a, const char *b) {
    if (a == NULL && b == NULL) {
        return 0;
    }
    if (a == NULL) {
        return -1;
    }
    if (b == NULL) {
        return 1;
    }
    return strcmp(a, b);
}

// Libération sécurisée
void safe_free(void *ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}