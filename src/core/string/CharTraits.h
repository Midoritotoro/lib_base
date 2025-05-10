#pragma once 

#include <base/core/string/StringConfig.h>
#include <base/core/utility/TypeTraits.h>

#include <base/core/string/Char.h>

__BASE_STRING_NAMESPACE_BEGIN

template <class _Char_>
class CharTraits;

template <>
class CharTraits<char32_t> {

};

template <>
class CharTraits<char16_t> {

};

#if __cpp_lib_char8_t

template <>
class CharTraits<char8_t> {

};

#endif

template <>
class CharTraits<wchar_t> {

};

template <>
class CharTraits<Char> {

};

__BASE_STRING_NAMESPACE_END