#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/BaseNamespace.h> 


__BASE_NAMESPACE_BEGIN

base_always_inline static
  bool UnsignedAddOverflow(
    unsigned a,
    unsigned b,
    unsigned* res)
{
#if defined(base_cpp_gnu) || defined(base_cpp_clang)
    return __builtin_uadd_overflow(a, b, res);
#else
    * res = a + b;
    return (a + b) < a;
#endif
}

base_always_inline static  
bool UnsignedLongAddOverflow(
    unsigned long a,
    unsigned long b,
    unsigned long* res)
{
#if defined(base_cpp_gnu) || defined(base_cpp_clang)
    return __builtin_uaddl_overflow(a, b, res);
#else
    * res = a + b;
    return (a + b) < a;
#endif
}

base_always_inline static  
bool UnsignedLongLongAddOverflow(
    unsigned long long a,
    unsigned long long b,
    unsigned long long* res)
{
#if defined(base_cpp_gnu) || defined(base_cpp_clang)
    return __builtin_uaddll_overflow(a, b, res);
#else
    * res = a + b;
    return (a + b) < a;
#endif
}

base_always_inline static  
bool SignedAddOverflow(
    int a,
    int b,
    int* res)
{
#if defined(base_cpp_gnu) || defined(base_cpp_clang)
    return __builtin_uadd_overflow(a, b, res);
#else
    * res = a + b;
    return (a + b) < a;
#endif
}

base_always_inline static  
bool SizetypeAddOverflow(
    sizetype a,
    sizetype b,
    sizetype* res)
{
    *res = a + b;
    return (a + b) < a;
}

#if !(defined(base_cpp_gnu) || defined(base_cpp_clang))
#include <numeric>
#endif

base_always_inline static  
bool UnsignedMultiplyOverflow(
    unsigned a,
    unsigned b,
    unsigned* res)
{
#if defined(base_cpp_gnu) || defined(base_cpp_clang)
    return __builtin_umul_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (UINT_MAX / b);
#endif
}

base_always_inline static  
bool UnsignedLongMultiplyOverflow(
    unsigned long a,
    unsigned long b,
    unsigned long* res)
{
#if defined(base_cpp_gnu) || defined(base_cpp_clang)
    return __builtin_umull_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (ULONG_MAX / b);
#endif
}

base_always_inline static  
bool UnsignedLongLongMultiplyOverflow(
    unsigned long long a,
    unsigned long long b,
    unsigned long long* res)
{
#if defined(base_cpp_gnu) || defined(base_cpp_clang)
    return __builtin_umulll_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (ULLONG_MAX / b);
#endif
}

base_always_inline static  
bool SignedMultiplyOverflow(
    int a,
    int b,
    int* res)
{
#if defined(base_cpp_gnu) || defined(base_cpp_clang)
    return __builtin_umull_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (INT_MAX / b);
#endif
}

base_always_inline static  
bool SignedMultiplyOverflow(
    int64 a,
    int64 b,
    int64* res)
{
#if defined(base_cpp_gnu) || defined(base_cpp_clang)
    return __builtin_umulll_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (LLONG_MAX / b);
#endif
}

base_always_inline static  
bool MultiplyOverflow(
    unsigned a,
    unsigned b,
    unsigned* res)
{
    return UnsignedMultiplyOverflow(a, b, res);
}

base_always_inline static  
bool MultiplyOverflow(
    unsigned long a,
    unsigned long b,
    unsigned long* res)
{
    return UnsignedLongMultiplyOverflow(a, b, res);
}

base_always_inline static  
bool MultiplyOverflow(
    unsigned long long a,
    unsigned long long b,
    unsigned long long* res)
{
    return UnsignedLongLongMultiplyOverflow(a, b, res);
}

base_always_inline static  
bool MultiplyOverflow(
    int a,
    int b,
    int* res)
{
    return SignedMultiplyOverflow(a, b, res);
}

base_always_inline static
 
bool MultiplyOverflow(
    int64 a,
    int64 b,
    int64* res)
{
    return SignedMultiplyOverflow(a, b, res);
}

base_always_inline static  
bool AdditionOverflow(
    int a,
    int b,
    int* res)
{
    return SignedAddOverflow(a, b, res);
}

base_always_inline static  
bool AdditionOverflow(
    unsigned a,
    unsigned b,
    unsigned* res)
{
    return UnsignedAddOverflow(a, b, res);
}

base_always_inline static  
bool AdditionOverflow(
    unsigned long a,
    unsigned long b,
    unsigned long* res)
{
    return UnsignedLongAddOverflow(a, b, res);
}

base_always_inline static  
bool AdditionOverflow(
    unsigned long long a,
    unsigned long long b,
    unsigned long long* res)
{
    return UnsignedLongLongAddOverflow(a, b, res);
}

inline  
void* MemoryAllocate(size_t count, size_t size)
{
    return MultiplyOverflow(count, size, &size)
        ? nullptr
        : malloc(count);
}

template <
    typename _Integer_,
    std::enable_if_t<std::is_integral_v<_Integer_>>>
base_always_inline static
  constexpr
bool AnyMultiplyOverflow(
    _Integer_ a,
    _Integer_ b,
    _Integer_* res) noexcept
{
    using _Limits_ = std::numeric_limits<_Integer_>;

    *res = a * b;
    return b > 0 && a > (_Limits_::max() / b);
}

__BASE_NAMESPACE_END