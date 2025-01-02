#pragma once 

enum VISIT {
    preorder,
    postorder,
    endorder,
    leaf
};

struct node_t {
    char* key;
    struct node_t* llink, * rlink;
};


typedef void (*cmp_fn_t)(const void*, VISIT, int);
struct node_t;

[[nodiscard]] void* tfind(
    const void* vkey,
    void* const* vrootp,
    int (*compar)(const void*,
        const void*));

[[nodiscard]] void* tsearch(
    const void* vkey,
    void** vrootp,
    int (*compar)(const void*,
        const void*));

void trecurse(
    const node_t* root,
    cmp_fn_t action, int level);

void twalk(
    const void* vroot,
    cmp_fn_t action);

int cmp_smallest(
    const void* a,
    const void* b);


void tdestroy_recurse(
    node_t* root,
    void (*free_action)(void*));

void tdestroy(
    void* root,
    void (*freenode)(void*));