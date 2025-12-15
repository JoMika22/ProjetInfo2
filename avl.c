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

AVL* rotationGauche(AVL* a) {
    AVL* pivot = a->fd;
    a->fd = pivot->fg; 
    pivot->fg = a;   
    int eq_a = a->equilibre;
    int eq_p = pivot->equilibre;
    if (eq_p > 0) {
        a->equilibre = eq_a - eq_p - 1;
    } else {
        a->equilibre = eq_a - 0 - 1;
    }
    if (a->equilibre < 0) {
        pivot->equilibre = eq_p + a->equilibre - 1;
    } else {
        pivot->equilibre = eq_p - 1; // + 0 - 1
    }
    return pivot;
}

AVL* rotationDroite(AVL* a) {
    AVL* pivot = a->fg;
    a->fg = pivot->fd; 
    pivot->fd = a;
    int eq_a = a->equilibre;
    int eq_p = pivot->equilibre;
    if (eq_p < 0) {
        a->equilibre = eq_a - eq_p + 1;
    } else {
        a->equilibre = eq_a - 0 + 1;
    }
    if (a->equilibre > 0) {
        pivot->equilibre = eq_p + a->equilibre + 1;
    } else {
        pivot->equilibre = eq_p + 1;
    }
    return pivot;
}

AVL* rotationDoubleGauche(AVL* a) {
    a->fd = rotationDroite(a->fd);
    return rotationGauche(a);
}

AVL* rotationDoubleDroite(AVL* a) {
    a->fg = rotationGauche(a->fg);
    return rotationDroite(a);
}

AVL* equilibrerAVL(AVL* a) {
    if (a->equilibre >= 2) {
        if (a->fd->equilibre >= 0) {
            return rotationGauche(a);
        } 
        else {
            return rotationDoubleGauche(a);
        }
    }
    if (a->equilibre <= -2) {
        if (a->fg->equilibre <= 0) {
            return rotationDroite(a);
        } 
        else {
            return rotationDoubleDroite(a);
        }
    }
    return a;
}


AVL* insereAVL(AVL* a, Station s, int *h){
    if (a == NULL){
        *h = 1;
        return creeAVL(s);
    }

    if (s.id < a->data.id){
        a->fg = insereAVL(a->fg, s, h);
        *h = -(*h);
    }
    else if (s.id > a->data.id){
        a->fd = insereAVL(a->fd, s, h);
    }
    else {
        a->data.capacite += s.capacite;
        a->data.consommation += s.consommation;
        *h = 0; 
        return a;
    }
    if (*h != 0){
        a->equilibre += *h;
        a = equilibrerAVL(a);

        if (a->equilibre == 0) {
            *h = 0;
        } else {
            *h = 1;
        }
    }
    return a;
}

void traiterInfixe(AVL* a) {
    if (a != NULL) {
        traiterInfixe(a->fg);
        printf("%d;%f;%f\n", a->data.id, a->data.capacity, a->data.consumption);
        traiterInfixe(a->fd);
    }
}

void libererAVL(AVL* a) {
    if (a != NULL) {
        libererAVL(a->fg);
        libererAVL(a->fd);
        free(a);
    }
}
