#pragma once 


namespace base {
    /* Ищет узел по ключу "vkey" в дереве "vrootp", или возвращает 0 */
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

    void TDestroy(
        void* root,
        void (*freenode)(void*));
} // namespace base