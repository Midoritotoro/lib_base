#pragma once 

#include <src/core/string/CharTypeCompatibility.h>

__BASE_STRING_NAMESPACE_BEGIN

enum class NarrowingConversionMode : uchar {
    DefaultReplacement  = 0,        // Replace unsupported chars with the default character
    CustomReplacement   = 0x01,     // Use the char provided by the user
    Skip                = 0x02      // Skip unsupported chars
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

class SkipConversionMode {
public:
    inline static constexpr auto narrowingConversionMode = NarrowingConversionMode::Skip;
};



inline constexpr auto defaultReplacementConversionMode = DefaultReplacementConversionMode{};

template <
    typename _Char_,
    _Char_ replacementCharacter>
inline constexpr auto customReplacementConversionMode = CustomReplacementConversionMode<_Char_, replacementCharacter>{};

inline constexpr auto skipConversionMode = SkipConversionMode{};


__BASE_STRING_NAMESPACE_END
