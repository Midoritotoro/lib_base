#include <base/utility/TFunctions.h>

#include <cstdlib>
#include <cassert>

#include <search.h>
#include <base/utility/Assert.h>


namespace base {
    struct node_t {
        char* key;
        struct node_t* llink, * rlink;
    };

    enum VISIT {
        preorder,
        postorder,
        endorder,
        leaf
    };

    typedef void (*cmp_fn_t)(const void*, VISIT, int);


    // $NetBSD: tdelete.c,v 1.8 2016/01/20 20:47:41 christos Exp $
    void* TDelete(
        const void* vkey,
        void** vrootp,
        int (*compar)(const void*, 
            const void*))
    {
        node_t** rootp = (node_t**)vrootp;
        node_t* p, * q, * r;
        int  cmp;

        Assert(vkey != NULL);
        Assert(compar != NULL);

        if (rootp == NULL || (p = *rootp) == NULL)
            return NULL;

        while ((cmp = (*compar)(vkey, (*rootp)->key)) != 0) {
            p = *rootp;
            rootp = (cmp < 0) ?
                &(*rootp)->llink :		/* follow llink branch */
                &(*rootp)->rlink;		/* follow rlink branch */
            if (*rootp == NULL)
                return NULL;		/* key not found */
        }
        r = (*rootp)->rlink;			/* D1: */
        if ((q = (*rootp)->llink) == NULL)	/* Left NULL? */
            q = r;
        else if (r != NULL) {			/* Right link is NULL? */
            if (r->llink == NULL) {		/* D2: Find successor */
                r->llink = q;
                q = r;
            }
            else {			/* D3: Find NULL link */
                for (q = r->llink; q->llink != NULL; q = r->llink)
                    r = q;
                r->llink = q->rlink;
                q->llink = (*rootp)->llink;
                q->rlink = (*rootp)->rlink;
            }
        }
        free(*rootp);				/* D4: Free node */
        *rootp = q;				/* link parent to new node */
        return p;
    }

    // $NetBSD: tfind.c,v 1.7 2012/06/25 22:32:45 abs Exp $
    [[nodiscard]] void* TFind(
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

    // $NetBSD: tsearch.c,v 1.7 2012/06/25 22:32:45 abs Exp $
    [[nodiscard]] void* TSearch(
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
                &(*rootp)->llink :		/* T3: Идем налево */
                &(*rootp)->rlink;		/* T4: Идем направо */
        }

        q = (node_t*)malloc(sizeof(node_t));		/* T5: Ключ не найден */
        if (q != 0) {				/* Создаем новый узел */
            *rootp = q;			/* Связываем его со старым */
            q->key = (char*)(vkey);	/* Инициализируем новый узел */
            q->llink = q->rlink = NULL;
        }
        return q;
    }

    void TRecurse(
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
                TRecurse(root->llink, action, level + 1);
            (*action)(root, postorder, level);
            if (root->rlink != NULL)
                TRecurse(root->rlink, action, level + 1);
            (*action)(root, endorder, level);
        }
    }

    // $NetBSD: twalk.c,v 1.4 2012/03/20 16:38:45 matt Exp $
    void TWalk(
        const void* vroot,
        cmp_fn_t action)
    {
        if (vroot != NULL && action != NULL)
            TRecurse((const node_t*)vroot, action, 0);
    }

    void TDestroyRecurse(
        node_t* root,
        void (*free_action)(void*))
    {
        if (root->llink != NULL)
            TDestroyRecurse(root->llink, free_action);
        if (root->rlink != NULL)
            TDestroyRecurse(root->rlink, free_action);

        (*free_action) ((void*)root->key);
        free(root);
    }

    void TDestroy(void* vrootp, void (*freefct)(void*))
    {
        node_t* root = (node_t*)vrootp;

        if (root != NULL)
            TDestroyRecurse(root, freefct);
    }
} // namespace base