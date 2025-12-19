#include "avl.h"
#include <stdlib.h>
#include <string.h>

static int max(int a, int b) { return a > b ? a : b; }

static AVLNode* new_node(const char *key, void *value) {
    AVLNode *node = malloc(sizeof(AVLNode));
    node->key = strdup(key);
    node->value = value;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

int avl_height(AVLNode *node) {
    return node ? node->height : 0;
}

static AVLNode* rotate_right(AVLNode *y) {
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(avl_height(y->left), avl_height(y->right)) + 1;
    x->height = max(avl_height(x->left), avl_height(x->right)) + 1;
    return x;
}

static AVLNode* rotate_left(AVLNode *x) {
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(avl_height(x->left), avl_height(x->right)) + 1;
    y->height = max(avl_height(y->left), avl_height(y->right)) + 1;
    return y;
}

static int get_balance(AVLNode *node) {
    return node ? avl_height(node->left) - avl_height(node->right) : 0;
}

AVLNode* avl_insert(AVLNode *node, const char *key, void *value, int (*cmp)(const char*, const char*)) {
    if (!node) return new_node(key, value);
    int c = cmp(key, node->key);
    if (c < 0)
        node->left = avl_insert(node->left, key, value, cmp);
    else if (c > 0)
        node->right = avl_insert(node->right, key, value, cmp);
    else
        return node;
    node->height = 1 + max(avl_height(node->left), avl_height(node->right));
    int balance = get_balance(node);
    if (balance > 1 && cmp(key, node->left->key) < 0)
        return rotate_right(node);
    if (balance < -1 && cmp(key, node->right->key) > 0)
        return rotate_left(node);
    if (balance > 1 && cmp(key, node->left->key) > 0) {
        node->left = rotate_left(node->left);
        return rotate_right(node);
    }
    if (balance < -1 && cmp(key, node->right->key) < 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }
    return node;
}

AVLNode* avl_find(AVLNode *node, const char *key, int (*cmp)(const char*, const char*)) {
    if (!node) return NULL;
    int c = cmp(key, node->key);
    if (c == 0) return node;
    if (c < 0) return avl_find(node->left, key, cmp);
    return avl_find(node->right, key, cmp);
}

void avl_inorder_reverse(AVLNode *node, void (*visit)(AVLNode*, void*), void *arg) {
    if (!node) return;
    avl_inorder_reverse(node->right, visit, arg);
    visit(node, arg);
    avl_inorder_reverse(node->left, visit, arg);
}

void avl_free(AVLNode *node, void (*free_value)(void*)) {
    if (!node) return;
    avl_free(node->left, free_value);
    avl_free(node->right, free_value);
    if (free_value) free_value(node->value);
    free(node->key);
    free(node);
}
