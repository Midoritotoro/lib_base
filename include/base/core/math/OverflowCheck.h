#pragma once 

#include <base/core/arch/Platform.h>
#include <base/core/BaseNamespace.h> 


__BASE_NAMESPACE_BEGIN

always_inline static
NODISCARD bool UnsignedAddOverflow(
    unsigned a,
    unsigned b,
    unsigned* res)
{
#if defined(CPP_GNU) || defined(CPP_CLANG)
    return __builtin_uadd_overflow(a, b, res);
#else
    * res = a + b;
    return (a + b) < a;
#endif
}

always_inline static NODISCARD
bool UnsignedLongAddOverflow(
    unsigned long a,
    unsigned long b,
    unsigned long* res)
{
#if defined(CPP_GNU) || defined(CPP_CLANG)
    return __builtin_uaddl_overflow(a, b, res);
#else
    * res = a + b;
    return (a + b) < a;
#endif
}

always_inline static NODISCARD
bool UnsignedLongLongAddOverflow(
    unsigned long long a,
    unsigned long long b,
    unsigned long long* res)
{
#if defined(CPP_GNU) || defined(CPP_CLANG)
    return __builtin_uaddll_overflow(a, b, res);
#else
    * res = a + b;
    return (a + b) < a;
#endif
}

always_inline static NODISCARD
bool SignedAddOverflow(
    int a,
    int b,
    int* res)
{
#if defined(CPP_GNU) || defined(CPP_CLANG)
    return __builtin_uaddll_overflow(a, b, res);
#else
    * res = a + b;
    return (a + b) < a;
#endif
}

always_inline static NODISCARD
bool SizetypeAddOverflow(
    sizetype a,
    sizetype b,
    sizetype* res)
{
    *res = a + b;
    return (a + b) < a;
}

#if !(defined(CPP_GNU) || defined(CPP_CLANG))
#include <numeric>
#endif

always_inline static NODISCARD
bool UnsignedMultiplyOverflow(
    unsigned a,
    unsigned b,
    unsigned* res)
{
#if defined(CPP_GNU) || defined(CPP_CLANG)
    return __builtin_umul_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (UINT_MAX / b);
#endif
}

always_inline static NODISCARD
bool UnsignedLongMultiplyOverflow(
    unsigned long a,
    unsigned long b,
    unsigned long* res)
{
#if defined(CPP_GNU) || defined(CPP_CLANG)
    return __builtin_umull_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (ULONG_MAX / b);
#endif
}

always_inline static NODISCARD
bool UnsignedLongLongMultiplyOverflow(
    unsigned long long a,
    unsigned long long b,
    unsigned long long* res)
{
#if defined(CPP_GNU) || defined(CPP_CLANG)
    return __builtin_umulll_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (ULLONG_MAX / b);
#endif
}

always_inline static NODISCARD
bool SignedMultiplyOverflow(
    int a,
    int b,
    int* res)
{
#if defined(CPP_GNU) || defined(CPP_CLANG)
    return __builtin_umulll_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (INT_MAX / b);
#endif
}

always_inline static NODISCARD
bool SignedMultiplyOverflow(
    int64 a,
    int64 b,
    int64* res)
{
#if defined(CPP_GNU) || defined(CPP_CLANG)
    return __builtin_umulll_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (LLONG_MAX / b);
#endif
}

always_inline static NODISCARD
bool MultiplyOverflow(
    unsigned a,
    unsigned b,
    unsigned* res)
{
    return UnsignedMultiplyOverflow(a, b, res);
}

always_inline static NODISCARD
bool MultiplyOverflow(
    unsigned long a,
    unsigned long b,
    unsigned long* res)
{
    return UnsignedLongMultiplyOverflow(a, b, res);
}

always_inline static NODISCARD
bool MultiplyOverflow(
    unsigned long long a,
    unsigned long long b,
    unsigned long long* res)
{
    return UnsignedLongLongMultiplyOverflow(a, b, res);
}

always_inline static NODISCARD
bool MultiplyOverflow(
    int a,
    int b,
    int* res)
{
    return SignedMultiplyOverflow(a, b, res);
}

always_inline static
NODISCARD
bool MultiplyOverflow(
    int64 a,
    int64 b,
    int64* res)
{
    return SignedMultiplyOverflow(a, b, res);
}

always_inline static NODISCARD
bool AdditionOverflow(
    int a,
    int b,
    int* res)
{
    return SignedAddOverflow(a, b, res);
}

always_inline static NODISCARD
bool AdditionOverflow(
    unsigned a,
    unsigned b,
    unsigned* res)
{
    return UnsignedAddOverflow(a, b, res);
}

always_inline static NODISCARD
bool AdditionOverflow(
    unsigned long a,
    unsigned long b,
    unsigned long* res)
{
    return UnsignedLongAddOverflow(a, b, res);
}

always_inline static NODISCARD
bool AdditionOverflow(
    unsigned long long a,
    unsigned long long b,
    unsigned long long* res)
{
    return UnsignedLongLongAddOverflow(a, b, res);
}

inline NODISCARD
void* MemoryAllocate(size_t count, size_t size)
{
    return MultiplyOverflow(count, size, &size)
        ? nullptr
        : malloc(count);
}

template <
    typename _Integer_,
    std::enable_if_t<std::is_integral_v<_Integer_>>>
always_inline static
NODISCARD constexpr
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