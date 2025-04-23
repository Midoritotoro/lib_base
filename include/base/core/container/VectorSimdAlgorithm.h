#pragma once 

#include <base/core/arch/Platform.h>

#include <base/core/utility/TypeTraits.h>
#include <base/core/memory/MemoryRange.h>

__BASE_CONTAINER_NAMESPACE_BEGIN


template <
    class _Type_,
    class _Element_>
struct VectorAlgorithmInFindIsSafeObjectPointers : 
    std::false_type
{};

template <
    class _Type1_,
    class _Type2_>
struct VectorAlgorithmInFindIsSafeObjectPointers<_Type1_*, _Type2_*>
    : std::conjunction<
          // Type1* is an object pointer type
        std::disjunction<
            std::is_object<_Type1_>,
            std::is_void<_Type1_>>,
          // Type2* is an object pointer type
        std::disjunction<
            std::is_object<_Type2_>,
            std::is_void<_Type2_>>,
          // either Type1 is the same as Type2 (ignoring cv-qualifiers), or one of the two is void
        std::disjunction<
            std::is_same<std::remove_cv_t<_Type1_>,
        std::remove_cv_t<_Type2_>>, 
            std::is_void<_Type1_>,
        std::is_void<_Type2_>>> 
{};

// Can we activate the vector algorithms to find a value in a range of elements?
template <
    class _Type_,
    class _Element_>
constexpr bool VectorAlgorithmInFindIsSafeElement = std::disjunction_v<
#ifdef __cpp_lib_byte
    // We're finding a std::byte in a range of std::byte.
    std::conjunction<std::is_same<_Type_, std::byte>, std::is_same<_Element_, std::byte>>,
#endif // defined(__cpp_lib_byte)
    // We're finding an integer in a range of integers.
    // This case is the one that requires careful runtime handling in CouldCompareEqualToValueType.
    std::conjunction<
        std::is_integral<_Type_>,
        std::is_integral<_Element_>>,
    // We're finding an (object or function) pointer in a range of pointers of the same type.
    std::conjunction<
        std::is_pointer<_Type_>, 
        std::is_same<_Type_, _Element_>>,
    // We're finding a nullptr in a range of (object or function) pointers.
    std::conjunction<
        std::is_same<_Type_, std::nullptr_t>,
        std::is_pointer<_Element_>>,
    // We're finding an object pointer in a range of object pointers, and:
    // - One of the pointer types is a cv void*.
    // - One of the pointer types is a cv1 U* and the other is a cv2 U*.
    VectorAlgorithmInFindIsSafeObjectPointers<_Type_, _Element_>>;

template <class _Iterator_> 
using IteratorValueType = typename _Iterator_::value_type;


// Can we activate the vector algorithms for find/count?
template <
    class _Iterator_,
    class _Type_>
constexpr bool VectorAlgorithmInFindIsSafe =
    // The iterator must be contiguous so we can get raw pointers.
    IsIteratorContiguous<_Iterator_>
    // The iterator must not be volatile.
    && !IsIteratorVolatile<_Iterator_>
    // The type of the value to find must be compatible with the type of the elements.
    && VectorAlgorithmInFindIsSafeElement<_Type_, IteratorValueType<_Iterator_>>;

struct NODISCARD ZeroUpperOnDelete {
    ZeroUpperOnDelete() = default;

    ZeroUpperOnDelete(const ZeroUpperOnDelete&) = delete;
    ZeroUpperOnDelete& operator=(const ZeroUpperOnDelete&) = delete;

    ~ZeroUpperOnDelete() {
        _mm256_zeroupper();
    }
};

__m256i _Avx2_tail_mask_32(const size_t _Count_in_dwords) noexcept {
    // _Count_in_dwords must be within [0, 8].
    static constexpr unsigned int _Tail_masks[16] = {
        ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, ~0u, 0, 0, 0, 0, 0, 0, 0, 0 };
    return _mm256_loadu_si256(reinterpret_cast<const __m256i*>(_Tail_masks + (8 - _Count_in_dwords)));
}

template <
    class _InputIterator_,
    class _Predicate_>
// Find first satisfying _Pred
CONSTEXPR_CXX20 inline NODISCARD _InputIterator_ FindIf(
    _InputIterator_             _First, 
    const _InputIterator_       _Last, 
    _Predicate_                 _Pred)
{
    auto _UFirst      = memory::UnFancy(_First);
    const auto _ULast = memory::UnFancy(_Last);

    for (; _UFirst != _ULast; ++_UFirst)
        if (_Pred(*_UFirst))
            break;

    _First = std::forward<_InputIterator_>(_UFirst);
    return _First;
}

template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD _InputIterator_ FindSSE2(
    _InputIterator_             _First, 
    const _InputIterator_       _Last,
    const _Type_&               _Value) 
{ 
    
}


template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD _InputIterator_ FindSSE42(
    _InputIterator_             _First, 
    const _InputIterator_       _Last,
    const _Type_&               _Value) 
{ 
    
}


template <
    class _Traits_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD _InputIterator_ FindAVX(
    const void* _First,
    const void* _Last,
    _Type_ _Val)
{ 
    const std::size_t _Size_bytes = memory::ByteLength(_First, _Last);

    if (const size_t _Avx_size = _Size_bytes & ~size_t{ 0x1F }; _Avx_size != 0 && _Use_avx2()) {
        ZeroUpperOnDelete _Guard;

        const __m256i _Comparand = _Traits::_Set_avx(_Val);
        const void* _Stop_at = _First;
        _Advance_bytes(_Stop_at, _Avx_size);

        do {
            const __m256i _Data = _mm256_loadu_si256(static_cast<const __m256i*>(_First));
            const int _Bingo = _mm256_movemask_epi8(_Traits::_Cmp_avx(_Data, _Comparand));

            if (_Bingo != 0) {
                const unsigned long _Offset = _tzcnt_u32(_Bingo);
                _Advance_bytes(_First, _Offset);
                return _First;
            }

            _Advance_bytes(_First, 32);
        } while (_First != _Stop_at);

        if (const size_t _Avx_tail_size = _Size_bytes & 0x1C; _Avx_tail_size != 0) {
            const __m256i _Tail_mask = _Avx2_tail_mask_32(_Avx_tail_size >> 2);
            const __m256i _Data = _mm256_maskload_epi32(static_cast<const int*>(_First), _Tail_mask);
            const int _Bingo =
                _mm256_movemask_epi8(_mm256_and_si256(_Traits::_Cmp_avx(_Data, _Comparand), _Tail_mask));

            if (_Bingo != 0) {
                const unsigned long _Offset = _tzcnt_u32(_Bingo);
                _Advance_bytes(_First, _Offset);
                return _First;
            }

            _Advance_bytes(_First, _Avx_tail_size);
        }

        if constexpr (sizeof(_Ty) >= 4) {
            return _First;
        }
    }
}


template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD _InputIterator_ FindAVX512(
    _InputIterator_             _First, 
    const _InputIterator_       _Last,
    const _Type_&               _Value) 
{ 
    
}



template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD _InputIterator_ Find(
    _InputIterator_             _First, 
    const _InputIterator_       _Last,
    const _Type_&               _Value)
{
    if (ProcessorFeatures::AVX512F())
        return FindAVX512(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeaturse::AVX())
        return FindAVX(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeatures::SSE42())
        return FindSSE42(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeatures::SSE2())
        return FindSSE2(std::move(_First), std::move(_Last), _Value);

    return _First;
}



// ===========================================================================


template <
    class _InputIterator_,
    class _Predicate_>
// Find first satisfying _Pred
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountIf(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    _Predicate_                 _Pred)
{
    std::size_t _Count = 0;

    auto _UFirst = memory::UnFancy(_First);
    const auto _ULast = memory::UnFancy(_Last);

    for (; _UFirst != _ULast; ++_UFirst)
        if (_Pred(*_UFirst))
            ++_Count;

    return _Count;
}

template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountSSE2(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    const _Type_&               _Value)
{

}


template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountSSE42(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    const _Type_&               _Value)
{

}


template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountAVX(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    const _Type_&               _Value)
{

}


template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t CountAVX512(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    const _Type_&               _Value)
{   

}

template <
    class _InputIterator_,
    class _Type_>
CONSTEXPR_CXX20 inline NODISCARD std::size_t Count(
    _InputIterator_             _First,
    const _InputIterator_       _Last,
    const _Type_&               _Value)
{
    if (ProcessorFeatures::AVX512F())
        return CountAVX512(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeaturse::AVX())
        return CountAVX(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeatures::SSE42())
        return CountSSE42(std::move(_First), std::move(_Last), _Value);
    else if (ProcessorFeatures::SSE2())
        return CountSSE2(std::move(_First), std::move(_Last), _Value);

    return find;
}


__BASE_CONTAINER_NAMESPACE_END