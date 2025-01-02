#pragma once 

#include "base/Utility.h"


namespace core::utility {
    [[nodiscard]] void* TFind(
        const void* vkey,
        void* const* vrootp,
        int (*compar)(const void*,
            const void*));

    [[nodiscard]] void* TSearch(
        const void* vkey,
        void** vrootp,
        int (*compar)(const void*,
            const void*));

    void TRecurse(
        const node_t* root,
        cmp_fn_t action, int level);

    void TWalk(
        const void* vroot,
        cmp_fn_t action);

    int CmpSmallest(
        const void* a,
        const void* b);

    void TDestroyRecurse(
        node_t* root,
        void (*free_action)(void*));

    void TDestroy(
        void* root,
        void (*freenode)(void*));

} // namespace core::utility