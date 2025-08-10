#pragma once 

#include <base/core/arch/Platform.h>

__BASE_MEMORY_NAMESPACE_BEGIN

base_declare_const_function   base_always_inline bool alwaysTrue(
    const char*,
    const char*) noexcept
{
    return true;
}


base_declare_const_function   base_always_inline bool memcmp1(
    const char* first,
    const char* second) noexcept 
{
    return first[0] == second[0];
}


base_declare_const_function   base_always_inline bool memcmp2(
    const char* first,
    const char* second) noexcept 
{
    const uint16 __first    = *reinterpret_cast<const uint16*>(first);
    const uint16 __second   = *reinterpret_cast<const uint16*>(second);

    return __first == __second;
}


base_declare_const_function   base_always_inline bool memcmp3(
    const char* first, 
    const char* second) noexcept 
{
    const uint32 __first    = *reinterpret_cast<const uint32*>(first);
    const uint32 __second   = *reinterpret_cast<const uint32*>(second);

    return (__first & 0x00ffffff) == (__second & 0x00ffffff);
}


base_declare_const_function   base_always_inline bool memcmp4(
    const char* first,
    const char* second) noexcept 
{
    const uint32 __first    = *reinterpret_cast<const uint32*>(first);
    const uint32 __second   = *reinterpret_cast<const uint32*>(second);

    return __first == __second;
}


base_declare_const_function   base_always_inline bool memcmp5(
    const char* first, 
    const char* second) noexcept 
{
    const uint64 __first    = *reinterpret_cast<const uint64*>(first);
    const uint64 __second   = *reinterpret_cast<const uint64*>(second);

    return ((__first ^ __second) & 0x000000fffffffffflu) == 0;
}


base_declare_const_function   base_always_inline bool memcmp6(
    const char* first, 
    const char* second) noexcept 
{
    const uint64 __first    = *reinterpret_cast<const uint64*>(first);
    const uint64 __second   = *reinterpret_cast<const uint64*>(second);

    return ((__first ^ __second) & 0x0000fffffffffffflu) == 0;
}


base_declare_const_function   base_always_inline bool memcmp7(
    const char* first, 
    const char* second) noexcept
{
    const uint64 __first    = *reinterpret_cast<const uint64*>(first);
    const uint64 __second   = *reinterpret_cast<const uint64*>(second);

    return ((__first ^ __second) & 0x00fffffffffffffflu) == 0;
}


base_declare_const_function   base_always_inline bool memcmp8(
    const char* first,
    const char* second) noexcept
{
    const uint64 __first    = *reinterpret_cast<const uint64*>(first);
    const uint64 __second   = *reinterpret_cast<const uint64*>(second);

    return __first == __second;
}


base_declare_const_function   base_always_inline bool memcmp9(
    const char* first,
    const char* second) noexcept
{
    const uint64 __first    = *reinterpret_cast<const uint64*>(first);
    const uint64 __second   = *reinterpret_cast<const uint64*>(second);

    return (__first == __second) & (first[8] == second[8]);
}


base_declare_const_function   base_always_inline bool memcmp10(
    const char* first, 
    const char* second) noexcept 
{
    const uint64 __firstQuad    = *reinterpret_cast<const uint64*>(first);
    const uint64 __secondQuad   = *reinterpret_cast<const uint64*>(second);

    const uint16 __firstWord    = *reinterpret_cast<const uint16*>(first + 8);
    const uint16 __secondWord   = *reinterpret_cast<const uint16*>(second + 8);

    return (__firstQuad == __secondQuad) & (__firstWord == __secondWord);
}


base_declare_const_function   base_always_inline bool memcmp11(
    const char* first,
    const char* second) noexcept 
{
    const uint64 __firstQuad      = *reinterpret_cast<const uint64*>(first);
    const uint64 __secondQuad     = *reinterpret_cast<const uint64*>(second);

    const uint32 __firstDWord     = *reinterpret_cast<const uint32*>(first + 8);
    const uint32 __secondDWord    = *reinterpret_cast<const uint32*>(second + 8);

    return (__firstQuad == __secondQuad) & ((__firstDWord & 0x00ffffff) == (__secondDWord & 0x00ffffff));
}


base_declare_const_function   base_always_inline bool memcmp12(
    const char* first,
    const char* second) noexcept
{
    const uint64 __firstQuad    = *reinterpret_cast<const uint64*>(first);
    const uint64 __secondQuad   = *reinterpret_cast<const uint64*>(second);

    const uint32 __firstDWord   = *reinterpret_cast<const uint32*>(first + 8);
    const uint32 __secondDWord  = *reinterpret_cast<const uint32*>(second + 8);

    return (__firstQuad == __secondQuad) & (__firstDWord == __secondDWord);
}

__BASE_MEMORY_NAMESPACE_END
