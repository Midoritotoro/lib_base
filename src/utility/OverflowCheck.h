#pragma once 

#include "Config.h"


/**
 * Проверка сложением на переполнение при выделении памяти 
 * \param a - Число
 * \param b - Число
 * \param r - Указатель на число
 * \retval false, если сумма не вызывает переполнения
 * \retval true, если сумма вызывает переполнение
 */

static always_inline bool UnsignedAddOverflow(
    unsigned a, 
    unsigned b, 
    unsigned* res)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_uadd_overflow(a, b, res);
#else
    * res = a + b;
    return (a + b) < a;
#endif
}

static always_inline bool UnsignedLongAddOverflow(
    unsigned long a,
    unsigned long b,
    unsigned long* res)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_uaddl_overflow(a, b, res);
#else
    * res = a + b;
    return (a + b) < a;
#endif
}

static always_inline bool UnsignedLongLongAddOverflow(
    unsigned long long a,
    unsigned long long b,
    unsigned long long* res)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_uaddll_overflow(a, b, res);
#else
    * res = a + b;
    return (a + b) < a;
#endif
}

// --------------------------------------------

/**
 * Проверка умножением на переполнение при выделении паияти
 * \param a - Число
 * \param b - Число
 * \param r - Указатель на число
 * \retval false, если произведение не вызывает переполнения
 * \retval true, если произведение вызывает переполнение
 */

#if !(defined(__GNUC__) || defined(__clang__))
#include <numeric>
#endif

static always_inline bool UnsignedMultiplyOverflow(
    unsigned a,
    unsigned b,
    unsigned* res)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_umul_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (UINT_MAX / b);
#endif
}

static always_inline bool UnsignedLongMultiplyOverflow(
    unsigned long a,
    unsigned long b,
    unsigned long* res)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_umull_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (ULONG_MAX / b);
#endif
}

static always_inline bool UnsignedLongLongMultiplyOverflow(
    unsigned long long a, 
    unsigned long long b,
    unsigned long long* res)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_umulll_overflow(a, b, res);
#else
    * res = a * b;
    return b > 0 && a > (ULLONG_MAX / b);
#endif
}

[[nodiscard]] always_inline bool MultiplyOverflow(
    unsigned a, 
    unsigned b,
    unsigned* res)
{
    return UnsignedMultiplyOverflow(a, b, res);
}

[[nodiscard]] always_inline bool MultiplyOverflow(
    unsigned long a,
    unsigned long b,
    unsigned long* res)
{
    return UnsignedLongMultiplyOverflow(a, b, res);
}

[[nodiscard]] always_inline bool MultiplyOverflow(
    unsigned long long a, 
    unsigned long long b,
    unsigned long long* res)
{
    return UnsignedLongLongMultiplyOverflow(a, b, res);
}

[[nodiscard]] always_inline bool AdditionOverflow(
    unsigned a,
    unsigned b,
    unsigned* res)
{
    return UnsignedAddOverflow(a, b, res);
}

[[nodiscard]] always_inline bool AdditionOverflow(
    unsigned long a, 
    unsigned long b,
    unsigned long* res)
{
    return UnsignedLongAddOverflow(a, b, res);
}

[[nodiscard]] always_inline bool AdditionOverflow(
    unsigned long long a,
    unsigned long long b,
    unsigned long long* res)
{
    return UnsignedLongLongAddOverflow(a, b, res);
}

[[nodiscard]] inline void* MemoryAllocate(size_t count, size_t size) {
    return MultiplyOverflow(count, size, &size) 
        ? nullptr 
        : malloc(count);
}
