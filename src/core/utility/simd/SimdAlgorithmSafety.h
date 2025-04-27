#pragma once 

#include <base/core/arch/Platform.h>

#include <base/core/utility/TypeTraits.h>
#include <base/core/memory/MemoryRange.h>

__BASE_NAMESPACE_BEGIN
std::_Iter_value_t<_st>

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

struct NODISCARD ZeroUpperOnDeleteAvx {
    ZeroUpperOnDeleteAvx() = default;

    ZeroUpperOnDeleteAvx(const ZeroUpperOnDeleteAvx&) = delete;
    ZeroUpperOnDeleteAvx& operator=(const ZeroUpperOnDeleteAvx&) = delete;

    ~ZeroUpperOnDeleteAvx() {
        _mm256_zeroupper();
    }
};

__BASE_NAMESPACE_END
