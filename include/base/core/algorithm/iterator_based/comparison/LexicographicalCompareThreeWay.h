#pragma once

#include <src/core/algorithm/AlgorithmDebug.h>


#if base_has_cxx20

__BASE_ALGORITHM_NAMESPACE_BEGIN

template <
    class _FirstInputIterator_,
    class _SecondInputIterator_, 
    class _CompareFunction_>
base_nodiscard constexpr auto lexicographical_compare_three_way(
    const _FirstInputIterator_      firstIterator1,
    const _FirstInputIterator_      lastIterator1,
    const _SecondInputIterator_     firstIterator2,
    const _SecondInputIterator_     lastIterator2,
    _CompareFunction_               compareFunction) 
        -> decltype(compareFunction(*firstIterator1, *firstIterator2))
{
    static_assert(
        std::common_comparison_category_t<decltype(
            compareFunction(*firstIterator1, *firstIterator2))> != void,
        "The result of the comparator must have comparison category type (N4988 [alg.three.way]/2)."
    );

#if !defined(NDEBUG)
    VerifyRange(firstIterator1, lastIterator1);
    VerifyRange(firstIterator2, firstIterator2);
#endif

    using _MemcmpFunction_ = type_traits::_Lexicographical_compare_three_way_memcmp_classify<
        _FirstInputIterator_, _SecondInputIterator_, _CompareFunction_>;

    if constexpr (!std::is_void_v<_MemcmpFunction_>) {
        if (!type_traits::is_constant_evaluated()) {
            const auto firstRangeLength     = IteratorsDifference(firstIterator1, lastIterator1);
            const auto secondRangeLength    = IteratorsDifference(firstIterator2, lastIterator2);

            const auto minimum = (std::min)(firstRangeLength, secondRangeLength);

            const auto first1Pointer = memory::ToAddress(firstIterator1);
            const auto first2Pointer = memory::ToAddress(firstIterator2);

            const size_t mismatchPosition = 0;
            // const size_t mismatchPosition = mismatch<sizeof(*first1Pointer)>(first1Pointer, first2Pointer, minimum);

            if (mismatchPosition == firstRangeLength)
                return mismatchPosition == secondRangeLength
                    ? std::strong_ordering::equal 
                    : std::strong_ordering::less;
            else if (mismatchPosition == secondRangeLength)
                return std::strong_ordering::greater;
            else {
                const auto firstValue   = first1Pointer[mismatchPosition];
                const auto secondValue  = first2Pointer[mismatchPosition];

#if defined(base_cpp_msvc)
                // Avoid one comparison
                __assume(firstValue != secondValue);
#endif
                return compareFunction(firstValue, secondValue);
            }
        }
    }

    for (;;) {
        if (firstIterator1 == lastIterator1)
            return firstIterator2 == lastIterator2 
                ? std::strong_ordering::equal 
                : std::strong_ordering::less;

        if (firstIterator2 == lastIterator2)
            return std::strong_ordering::greater;

        if (const auto compareResult = compareFunction(*firstIterator1, *firstIterator2); compareResult != 0)
            return compareResult;

        ++firstIterator1;
        ++firstIterator2;
    }
}

template <
    class _FirstInputIterator_,
    class _SecondInputIterator_>
base_nodiscard constexpr auto lexicographical_compare_three_way(
    const _FirstInputIterator_  firstIterator1,
    const _FirstInputIterator_  lastIterator1,
    const _SecondInputIterator_ firstIterator2,
    const _SecondInputIterator_ lastIterator2)
{
    return base::algorithm::lexicographical_compare_three_way(
        firstIterator1, lastIterator1, 
        firstIterator2, lastIterator2, 
        std::compare_three_way{}
    );
}

__BASE_ALGORITHM_NAMESPACE_END

#endif // base_has_cxx20
