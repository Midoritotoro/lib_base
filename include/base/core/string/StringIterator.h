#pragma once 


#include <base/core/string/StringConfig.h>
#include <base/core/string/String.h>

#include <base/core/string/StringConstIterator.h>


__BASE_STRING_NAMESPACE_BEGIN

template <class _String = String>
class StringIterator: 
    public StringConstIterator<_String>
{
    using BaseClass         = StringConstIterator<_String>
public:
    using iterator_category = std::random_access_iterator_tag;

    using value_type        = typename _String::value_type;
    using difference_type   = typename _String::difference_type;

    using pointer           = typename _String::pointer;
    using reference         = value_type&;

    using BaseClass::BaseClass;


    _NODISCARD _CONSTEXPR20 reference operator*() const noexcept {
        return const_cast<reference>(BaseClass::operator*());
    }

    _CONSTEXPR20 Self& operator++() noexcept {
        BaseClass::operator++();
        return *this;
    }

    _CONSTEXPR20 StringIterator operator++(int) noexcept {
        StringIterator _Tmp = *this;
        BaseClass::operator++();
        return _Tmp;
    }

    _CONSTEXPR20 StringIterator& operator--() noexcept {
        BaseClass::operator--();
        return *this;
    }

    _CONSTEXPR20 StringIterator operator--(int) noexcept {
        StringIterator _Tmp = *this;
        BaseClass::operator--();
        return _Tmp;
    }

    _CONSTEXPR20 StringIterator& operator+=(const difference_type _Off) noexcept {
        BaseClass::operator+=(_Off);
        return *this;
    }

    _NODISCARD _CONSTEXPR20 StringIterator operator+(const difference_type _Off) const noexcept {
        StringIterator _Tmp = *this;
        _Tmp += _Off;
        return _Tmp;
    }

    _NODISCARD_FRIEND _CONSTEXPR20 StringIterator operator+(
        const difference_type _Off, StringIterator _Next) noexcept {
        _Next += _Off;
        return _Next;
    }

    _CONSTEXPR20 StringIterator& operator-=(const difference_type _Off) noexcept {
        BaseClass::operator-=(_Off);
        return *this;
    }

    using BaseClass::operator-;

    _NODISCARD _CONSTEXPR20 StringIterator operator-(const difference_type _Off) const noexcept {
        StringIterator _Tmp = *this;
        _Tmp -= _Off;
        return _Tmp;
    }

    _NODISCARD _CONSTEXPR20 reference operator[](const difference_type _Off) const noexcept {
        return const_cast<reference>(BaseClass::operator[](_Off));
    }
};

__BASE_STRING_NAMESPACE_END
