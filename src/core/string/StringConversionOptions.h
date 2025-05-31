#pragma once 

#include <src/core/string/CharTypeCompatibility.h>

__BASE_STRING_NAMESPACE_BEGIN

// Behavior during narrowing string conversion. For example, from char16_t to char
enum class NarrowingConversionMode : uchar {
    DefaultReplacement  = 0,        // Replace unsupported chars with the default character
    CustomReplacement   = 0x01,     // Use the char provided by the user
};

class DefaultReplacementConversionMode {
public:
    inline static constexpr auto narrowingConversionMode = NarrowingConversionMode::DefaultReplacement;
    inline static constexpr auto replacementCharacter = '?';
};

template <
    typename _Char_, 
    _Char_ replacementCharacter,
    std::enable_if_t<IsCompatibleCharType<_Char_>::value>>
class CustomReplacementConversionMode {
public:
    inline static constexpr auto narrowingConversionMode = NarrowingConversionMode::CustomReplacement;
    inline static constexpr auto replacementCharacter = replacementCharacter;
};

inline constexpr auto defaultReplacementConversionMode = DefaultReplacementConversionMode{};

template <
    typename _Char_,
    _Char_ replacementCharacter>
inline constexpr auto customReplacementConversionMode = CustomReplacementConversionMode<_Char_, replacementCharacter>{};


__BASE_STRING_NAMESPACE_END
