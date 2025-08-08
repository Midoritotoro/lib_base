#pragma once 


#include <base/core/compatibility/Compatibility.h>
#include <base/core/BaseNamespace.h>

__BASE_STRING_NAMESPACE_BEGIN

struct CharacterHelper
{
public:
    base_constexpr_cxx20 inline explicit CharacterHelper(char c) noexcept:
        ch(c) 
    {}

    base_constexpr_cxx20 inline base_nodiscard
        char toChar() const noexcept
    { 
        return ch; 
    }

    base_constexpr_cxx20 inline base_nodiscard
        char16_t unicode() const noexcept
    { 
        return char16_t(uchar(ch));
    }

    friend base_constexpr_cxx20 inline bool 
        operator==(
            CharacterHelper lhs,
            CharacterHelper rhs) noexcept
    {
        return lhs.ch == rhs.ch; 
    }

    friend base_constexpr_cxx20 inline bool 
        operator!=(
            CharacterHelper lhs,
            CharacterHelper rhs) noexcept
    {
        return lhs.ch != rhs.ch; 
    }

    friend base_constexpr_cxx20 inline bool 
        operator<=(
            CharacterHelper lhs,
            CharacterHelper rhs) noexcept 
    { 
        return lhs.ch <= rhs.ch;
    }

    friend base_constexpr_cxx20 inline bool 
        operator>=(
            CharacterHelper lhs, 
            CharacterHelper rhs) noexcept 
    { 
        return lhs.ch >= rhs.ch; 
    }

    friend base_constexpr_cxx20 inline bool 
        operator< (
            CharacterHelper lhs,
            CharacterHelper rhs) noexcept
    { 
        return lhs.ch < rhs.ch; 
    }

    friend base_constexpr_cxx20 inline bool 
        operator> (
            CharacterHelper lhs, 
            CharacterHelper rhs) noexcept
    { 
        return lhs.ch > rhs.ch;
    }

    friend base_constexpr_cxx20 inline bool 
        operator==(
            char lhs, 
            CharacterHelper rhs) noexcept 
    {
        return lhs == rhs.toChar();
    }

    friend base_constexpr_cxx20 inline bool 
        operator!=(
            char lhs, CharacterHelper rhs) noexcept
    {
        return lhs != rhs.toChar();
    }

    friend base_constexpr_cxx20 inline bool 
        operator<=(
            char lhs,
            CharacterHelper rhs) noexcept 
    { 
        return lhs <= rhs.toChar();
    }

    friend base_constexpr_cxx20 inline bool 
        operator>=(
            char lhs,
            CharacterHelper rhs) noexcept
    { 
        return lhs >= rhs.toChar();
    }

    friend base_constexpr_cxx20 inline bool
        operator< (
            char lhs,
            CharacterHelper rhs) noexcept
    { 
        return lhs < rhs.toChar();
    }

    friend base_constexpr_cxx20 inline bool 
        operator> (
            char lhs,
            CharacterHelper rhs) noexcept 
    { 
        return lhs > rhs.toChar();
    }

    friend base_constexpr_cxx20 inline bool 
        operator==(
            CharacterHelper lhs, 
            char rhs) noexcept 
    {
        return lhs.toChar() == rhs;
    }

    friend base_constexpr_cxx20 inline bool 
        operator!=(
            CharacterHelper lhs, 
            char rhs) noexcept 
    { 
        return lhs.toChar() != rhs;
    }

    friend base_constexpr_cxx20 inline bool 
        operator<=(
            CharacterHelper lhs,
            char rhs) noexcept
    { 
        return lhs.toChar() <= rhs;
    }

    friend base_constexpr_cxx20 inline bool
        operator>=(
            CharacterHelper lhs, 
            char rhs) noexcept
    { 
        return lhs.toChar() >= rhs;
    }

    friend base_constexpr_cxx20 inline bool 
        operator< (
            CharacterHelper lhs, 
            char rhs) noexcept 
    { 
        return lhs.toChar() < rhs;
    }

    friend base_constexpr_cxx20 inline bool
        operator> (
            CharacterHelper lhs, 
            char rhs) noexcept 
    {
        return lhs.toChar() > rhs;
    }
private:
    char ch;
};

__BASE_STRING_NAMESPACE_END
