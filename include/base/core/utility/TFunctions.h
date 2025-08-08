#pragma once 

#include <base/core/BaseNamespace.h>
#include <base/core/compatibility/Compatibility.h>


__BASE_NAMESPACE_BEGIN

/* Ищет узел по ключу "vkey" в дереве "vrootp", или возвращает 0 */
base_nodiscard void* TFind(
    const void* vkey,
    void* const* vrootp,
    int (*compar)(const void*,
        const void*));

base_nodiscard void* TSearch(
    const void* vkey,
    void** vrootp,
    int (*compar)(const void*,
        const void*));

void TDestroy(
    void* root,
    void (*freenode)(void*));

__BASE_NAMESPACE_END