#pragma once 

#include <base/core/utility/TypeTraits.h>
#include <base/core/utility/OverflowCheck.h>


__BASE_STRING_NAMESPACE_BEGIN

inline void* smartRealloc(
    void*           pointer,
    const size_t    currentSize,
    const size_t    currentCapacity,
    const size_t    newCapacity)
{
    DebugAssert(pointer);
    DebugAssert(currentSize <= currentCapacity && currentCapacity < newCapacity);

    auto const slack = currentCapacity - currentSize;

    if (slack * 2 > currentSize) {
        // Too much slack, malloc-copy-free cycle:
        auto const result = malloc(newCapacity);
        std::memcpy(result, pointer, currentSize);

        free(pointer);
        return result;
    }

    // If there's not too much slack, we realloc in hope of coalescing
    return realloc(pointer, newCapacity);
}


template <
    class _InputIterator_, 
    class _OutIterator_>
inline std::pair<_InputIterator_, _OutIterator_> copy_n(
    _InputIterator_                         b,
    IteratorDifferenceType<_InputIterator_> n,
    _OutIterator_                           d)
{
    for (; n != 0; --n, ++b, ++d)
        *d = *b;

    return std::make_pair(b, d);
}

template <
    class _Pod_, 
    class _Type_>
inline void podFill(
    _Pod_* b, 
    _Pod_* e, 
    _Type_ c) 
{
    DebugAssert(b && e && b <= e);
    constexpr auto kUseMemset = sizeof(_Type_) == 1;

    if (kUseMemset)
        memset(b, c, size_t(e - b));
    else {
        auto const ee = b + ((e - b) & ~7u);
        for (; b != ee; b += 8) {
            b[0] = c;
            b[1] = c;
            b[2] = c;
            b[3] = c;
            b[4] = c;
            b[5] = c;
            b[6] = c;
            b[7] = c;
        }
        // Leftovers
        for (; b != e; ++b)
            *b = c;
    }
}

template <class _Pod_>
inline void podCopy(
    const _Pod_*    b,
    const _Pod_*    e,
    _Pod_*          d)
{
    DebugAssert(b != nullptr);
    DebugAssert(e != nullptr);
    DebugAssert(d != nullptr);

    DebugAssert(e >= b);
    DebugAssert(d >= e || d + (e - b) <= b);

    memcpy(d, b, (e - b) * sizeof(_Pod_));
}


template <class _Pod_>
inline void podMove(
    const _Pod_* b, 
    const _Pod_* e,
    _Pod_* d)
{
    DebugAssert(e >= b);
    memmove(d, b, (e - b) * sizeof(*b));
}

__BASE_STRING_NAMESPACE_END
