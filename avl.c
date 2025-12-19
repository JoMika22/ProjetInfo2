#include "avl.h"
#include <stdlib.h>
#include <string.h>

/* retourne le maximum entre a et b (sans ternaire) */
static int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

/* crée un nouveau nœud et copie la clé */
static AVLNode* new_node(const char *key, void *value) {
    AVLNode *n = (AVLNode*)malloc(sizeof(AVLNode));
    if (n == NULL) return NULL;

    size_t len = strlen(key) + 1;
    n->key = (char*)malloc(len);
    if (n->key == NULL) {
        free(n);
        return NULL;
    }
    strcpy(n->key, key);

    n->value = value;
    n->height = 1;
    n->left = NULL;
    n->right = NULL;
    return n;
}

/* hauteur : 0 si NULL */
int avl_height(AVLNode *node) {
    if (node == NULL) {
        return 0;
    } else {
        return node->height;
    }
}

/* rotation droite */
static AVLNode* rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *tmp = x->right;

    x->right = y;
    y->left = tmp;

    y->height = 1 + max(avl_height(y->left), avl_height(y->right));
    x->height = 1 + max(avl_height(x->left), avl_height(x->right));
    return x;
}

/* rotation gauche */
static AVLNode* rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *tmp = y->left;

    y->left = x;
    x->right = tmp;

    x->height = 1 + max(avl_height(x->left), avl_height(x->right));
    y->height = 1 + max(avl_height(y->left), avl_height(y->right));
    return y;
}

/* différence de hauteur gauche - droite */
static int get_balance(AVLNode *n) {
    if (n == NULL) {
        return 0;
    }
    return avl_height(n->left) - avl_height(n->right);
}

/* insertion équilibrée (AVL). cmp doit comparer deux clés et retourner <0,0,>0 */
AVLNode* avl_insert(AVLNode *node, const char *key, void *value, int (*cmp)(const char*, const char*)) {
    if (node == NULL) {
        return new_node(key, value);
    }

    int c = cmp(key, node->key);
    if (c < 0) {
        node->left = avl_insert(node->left, key, value, cmp);
    } else if (c > 0) {
        node->right = avl_insert(node->right, key, value, cmp);
    } else {
        /* clé déjà présente : on ne change rien */
        return node;
    }

    /* mettre à jour la hauteur */
    node->height = 1 + max(avl_height(node->left), avl_height(node->right));

    /* vérifier l'équilibre et appliquer rotations simples/ doubles */
    int balance = get_balance(node);

    /* gauche-gauche */
    if (balance > 1) {
        /* si la clé est plus petite que la clé du fils gauche => GG */
        if (cmp(key, node->left->key) < 0) {
            return rotate_right(node);
        }
        /* sinon GD */
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    /* droite-droite ou droite-gauche */
    if (balance < -1) {
        /* DD */
        if (cmp(key, node->right->key) > 0) {
            return rotate_left(node);
        }
        /* DG */
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

/* recherche itérative d'une clé (retourne le nœud ou NULL) */
AVLNode* avl_find(AVLNode *node, const char *key, int (*cmp)(const char*, const char*)) {
    AVLNode *cur = node;
    while (cur != NULL) {
        int c = cmp(key, cur->key);
        if (c == 0) {
            return cur;
        } else if (c < 0) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    return NULL;
}

/* parcours en ordre inverse (du plus grand au plus petit) */
void avl_inorder_reverse(AVLNode *node, void (*visit)(AVLNode*, void*), void *arg) {
    if (node == NULL) return;
    avl_inorder_reverse(node->right, visit, arg);
    visit(node, arg);
    avl_inorder_reverse(node->left, visit, arg);
}

/* libère tout l'arbre ; free_value peut être NULL */
void avl_free(AVLNode *node, void (*free_value)(void*)) {
    if (node == NULL) return;
    avl_free(node->left, free_value);
    avl_free(node->right, free_value);
    if (free_value != NULL && node->value != NULL) {
        free_value(node->value);
    }
    if (node->key != NULL) free(node->key);
    free(node);
}
