#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

AVL* creeAVL(Station s){
    AVL* nouveau = malloc(sizeof(AVL));
    if (nouveau == NULL){
        perror("Erreur allocation mémoire");
        exit(1);
    }
    nouveau->data = s;
    nouveau->fg = NULL;
    nouveau->fd = NULL;
    nouveau->equilibre = 0;
    return nouveau;
}

AVL* rotationGauche(AVL* a){
    AVL* pivot = a->fd;
    a->fd = pivot->fg;
    pivot->fg = a;
    int eq_a = a->equilibre;
    int eq_p = pivot->equilibre;
    
}
