#pragma once 

#include <base/core/arch/Platform.h>

__BASE_STRING_NAMESPACE_BEGIN

DECLARE_NOALIAS NODISCARD const char* __base_strstrnSse2(
    const char* string,
    size_t      stringLength,
    const char* needle,
    size_t      needleLength) noexcept;

DECLARE_NOALIAS NODISCARD const char* __base_strstrnAvx2(
    const char* string,
    size_t      stringLength,
    const char* needle,
    size_t      needleLength) noexcept;

DECLARE_NOALIAS NODISCARD const char* __base_strstrnAvx512F(
    const char* string,
    size_t      stringLength,
    const char* needle,
    size_t      needleLength) noexcept;

// =================================================================

DECLARE_NOALIAS NODISCARD const wchar_t* __base_wcsstrnSse2(
    const wchar_t*  string,
    size_t          stringLength,
    const wchar_t*  needle,
    size_t          needleLength) noexcept;

DECLARE_NOALIAS NODISCARD const wchar_t* __base_wcsstrnAvx2(
    const wchar_t*  string,
    size_t          stringLength,
    const wchar_t*  needle,
    size_t          needleLength) noexcept;

DECLARE_NOALIAS NODISCARD const wchar_t* __base_wcsstrnAvx512F(
    const wchar_t*  string,
    size_t          stringLength,
    const wchar_t*  needle,
    size_t          needleLength) noexcept;

// Scalar 

DECLARE_NOALIAS NODISCARD const wchar_t* CDECL __base_wcsstrnScalar(
    const wchar_t*  string,
    size_t          stringLength,
    const wchar_t*  needle,
    size_t          needleLength) noexcept;

DECLARE_NOALIAS NODISCARD const char* CDECL __base_strstrnScalar(
    const char* string,
    size_t      stringLength,
    const char* needle,
    size_t      needleLength) noexcept;

__BASE_STRING_NAMESPACE_END
