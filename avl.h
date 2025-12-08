#ifndef AVL_H
#define AVL_H

typedef struct {
    int id;
    float capacite;
    float consommation;
} Station;

typedef struct AVL {
    Station data;           
    struct AVL *fg;
    struct AVL *fd;
    int equilibre;          
} AVL;

AVL* creeAVL(Station s);
AVL* insereAVL(AVL* a, Station s, int *h);
void traiterInfixe(AVL* a);
void libererAVL(AVL* a);

AVL* rotationGauche(AVL* a);
AVL* rotationDroite(AVL* a);
AVL* rotationDoubleGauche(AVL* a);
AVL* rotationDoubleDroite(AVL* a);
AVL* equilibrerAVL(AVL* a);

#endif


  
