#pragma once 


#include <base/core/arch/KeywordSupport.h>
#include <base/core/BaseNamespace.h>

__BASE_STRING_NAMESPACE_BEGIN

struct CharacterHelper
{
public:
    CONSTEXPR_CXX20 inline explicit CharacterHelper(char c) noexcept:
        ch(c) 
    {}

    CONSTEXPR_CXX20 inline char toChar() const noexcept { 
        return ch; 
    }

    CONSTEXPR_CXX20 inline char16_t unicode() const noexcept { 
        return char16_t(uchar(ch));
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator==(
            CharacterHelper lhs,
            CharacterHelper rhs) noexcept
    {
        return lhs.ch == rhs.ch; 
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator!=(
            CharacterHelper lhs,
            CharacterHelper rhs) noexcept
    {
        return lhs.ch != rhs.ch; 
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator<=(
            CharacterHelper lhs,
            CharacterHelper rhs) noexcept 
    { 
        return lhs.ch <= rhs.ch;
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator>=(
            CharacterHelper lhs, 
            CharacterHelper rhs) noexcept 
    { 
        return lhs.ch >= rhs.ch; 
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator< (
            CharacterHelper lhs,
            CharacterHelper rhs) noexcept
    { 
        return lhs.ch < rhs.ch; 
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator> (
            CharacterHelper lhs, 
            CharacterHelper rhs) noexcept
    { 
        return lhs.ch > rhs.ch;
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator==(
            char lhs, 
            CharacterHelper rhs) noexcept 
    {
        return lhs == rhs.toLatin1(); 
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator!=(
            char lhs, CharacterHelper rhs) noexcept
    {
        return lhs != rhs.toLatin1(); 
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator<=(
            char lhs,
            CharacterHelper rhs) noexcept 
    { 
        return lhs <= rhs.toLatin1(); 
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator>=(
            char lhs,
            CharacterHelper rhs) noexcept
    { 
        return lhs >= rhs.toLatin1();
    }

    friend CONSTEXPR_CXX20 inline bool
        operator< (
            char lhs,
            CharacterHelper rhs) noexcept
    { 
        return lhs < rhs.toLatin1(); 
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator> (
            char lhs,
            CharacterHelper rhs) noexcept 
    { 
        return lhs > rhs.toLatin1(); 
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator==(
            CharacterHelper lhs, 
            char rhs) noexcept 
    {
        return lhs.toLatin1() == rhs;
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator!=(
            CharacterHelper lhs, 
            char rhs) noexcept 
    { 
        return lhs.toLatin1() != rhs;
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator<=(
            CharacterHelper lhs,
            char rhs) noexcept
    { 
        return lhs.toLatin1() <= rhs;
    }

    friend CONSTEXPR_CXX20 inline bool
        operator>=(
            CharacterHelper lhs, 
            char rhs) noexcept
    { 
        return lhs.toLatin1() >= rhs;
    }

    friend CONSTEXPR_CXX20 inline bool 
        operator< (
            CharacterHelper lhs, 
            char rhs) noexcept 
    { 
        return lhs.toLatin1() < rhs;
    }

    friend CONSTEXPR_CXX20 inline bool
        operator> (
            CharacterHelper lhs, 
            char rhs) noexcept 
    {
        return lhs.toLatin1() > rhs; 
    }
private:
    char ch;
};

__BASE_STRING_NAMESPACE_END
