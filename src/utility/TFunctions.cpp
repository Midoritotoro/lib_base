#include "TFunctions.h"

#include <cstdlib>
#include <cassert>

#include <search.h>


static struct
{
    const void** tab;
    size_t count;
} list = { NULL, 0 };
static const void* smallest;


[[nodiscard]] void* tfind(
    const void* vkey,
    void* const* vrootp,
    int (*compar)(const void*,
        const void*))
{
    node_t* const* rootp = (node_t* const*)vrootp;

    assert(vkey != NULL);
    assert(compar != NULL);

    if (rootp == NULL)
        return NULL;

    while (*rootp != NULL) {		/* T1: */
        int r;

        if ((r = (*compar)(vkey, (*rootp)->key)) == 0)	/* T2: */
            return *rootp;		/* Ключ найден */
        rootp = (r < 0) ?
            &(*rootp)->llink :		/* T3: Идем на левую ветку */
            &(*rootp)->rlink;		/* T4: Идем на правую ветку */
    }
    return NULL;
}

[[nodiscard]] void* tsearch(
    const void* vkey,
    void** vrootp,
    int (*compar)(const void*,
        const void*))
{
    node_t* q;
    node_t** rootp = (node_t**)vrootp;

    assert(vkey != NULL);
    assert(compar != NULL);

    if (rootp == NULL)
        return NULL;

    while (*rootp != NULL) {	/* T1: */
        int r;

        if ((r = (*compar)(vkey, (*rootp)->key)) == 0)	/* T2: */
            return *rootp;		/* Ключ найден */

        rootp = (r < 0) ?
            &(*rootp)->llink :		/* T3: follow left branch */
            &(*rootp)->rlink;		/* T4: follow right branch */
    }

    q = (node_t*)malloc(sizeof(node_t));		/* T5: Ключ не найден */
    if (q != 0) {				/* Создаем новый узел */
        *rootp = q;			/* Связываем его со старым */
        q->key = (char*)(vkey);	/* Инициализируем новый узел */
        q->llink = q->rlink = NULL;
    }
    return q;
}

void trecurse(
    const node_t* root,
    cmp_fn_t action,
    int level)
{
    assert(root != NULL);
    assert(action != NULL);

    if (root->llink == NULL && root->rlink == NULL)
        (*action)(root, leaf, level);
    else {
        (*action)(root, preorder, level);
        if (root->llink != NULL)
            trecurse(root->llink, action, level + 1);
        (*action)(root, postorder, level);
        if (root->rlink != NULL)
            trecurse(root->rlink, action, level + 1);
        (*action)(root, endorder, level);
    }
}

/**
 *Обходит узлы дерева 
 *\param vroot - Дерево
 *\param action - Узел дерева для обхода
 */
void twalk(
    const void* vroot,
    cmp_fn_t action)
{
    if (vroot != NULL && action != NULL)
        trecurse((const node_t*)vroot, action, 0);
}


int cmp_smallest(
    const void* a,
    const void* b)
{
    if (a == b)
        return 0;
    if (a == smallest)
        return -1;
    if (b == smallest)
        return +1;
    abort();
}

void tdestroy_recurse(
    node_t* root,
    void (*free_action)(void*))
{
    if (root->llink != NULL)
        tdestroy_recurse(root->llink, free_action);
    if (root->rlink != NULL)
        tdestroy_recurse(root->rlink, free_action);

    (*free_action) ((void*)root->key);
    free(root);
}

void tdestroy(void* vrootp, void (*freefct)(void*))
{
    node_t* root = (node_t*)vrootp;

    if (root != NULL)
        tdestroy_recurse(root, freefct);
}
