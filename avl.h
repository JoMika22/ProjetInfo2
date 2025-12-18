#ifndef AVL_H
#define AVL_H

// Structure d'un noeud AVL générique

typedef struct AVLNode {
    char *key;
    void *value;
    int height;
    struct AVLNode *left;
    struct AVLNode *right;
} AVLNode;

AVLNode* avl_insert(AVLNode *node, const char *key, void *value, int (*cmp)(const char*, const char*));
AVLNode* avl_find(AVLNode *node, const char *key, int (*cmp)(const char*, const char*));
void avl_inorder_reverse(AVLNode *node, void (*visit)(AVLNode*, void*), void *arg);
void avl_free(AVLNode *node, void (*free_value)(void*));
int avl_height(AVLNode *node);

#endif // AVL_H
