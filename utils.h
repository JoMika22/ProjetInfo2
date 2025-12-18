#ifndef UTILS_H
#define UTILS_H

#include <string.h>

// Comparaison de chaînes
int cmp_str(const char *a, const char *b);

// Libération sécurisée
void safe_free(void *ptr);

#endif // UTILS_H
