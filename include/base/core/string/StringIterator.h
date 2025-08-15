#pragma once 


#include <base/core/string/StringConfig.h>
#include <base/core/string/StringConstIterator.h>


__BASE_STRING_NAMESPACE_BEGIN

template <class _String>
class StringIterator: 
    public StringConstIterator<_String>
{
public:
    using BaseClass = StringConstIterator<_String>;
    using iterator_category = std::random_access_iterator_tag;

    using value_type        = typename _String::value_type;
    using difference_type   = typename _String::difference_type;

    using pointer           = typename _String::pointer;
    using size_type         = typename _String::size_type;

    using reference         = value_type&;

    using BaseClass::BaseClass;

    base_constexpr_cxx20 reference operator*() const noexcept {
        return const_cast<reference>(BaseClass::operator*());
    }

    base_constexpr_cxx20 StringIterator& operator++() noexcept {
        BaseClass::operator++();
        return *this;
    }

    base_constexpr_cxx20 StringIterator operator++(int) noexcept {
        StringIterator temp = *this;
        BaseClass::operator++();

        return temp;
    }

    base_constexpr_cxx20 StringIterator& operator--() noexcept {
        BaseClass::operator--();
        return *this;
    }

    base_constexpr_cxx20 StringIterator operator--(int) noexcept {
        StringIterator temp = *this;
        BaseClass::operator--();

        return temp;
    }

    base_constexpr_cxx20 StringIterator& operator+=(const difference_type offset) noexcept {
        BaseClass::operator+=(offset);
        return *this;
    }

      base_constexpr_cxx20 StringIterator operator+(const difference_type offset) const noexcept {
        StringIterator temp = *this;
        temp += offset;

        return temp;
    }

      friend base_constexpr_cxx20 StringIterator operator+(
        const difference_type offset,
        StringIterator next) noexcept
    {
        next += offset;
        return next;
    }

    base_constexpr_cxx20 StringIterator& operator-=(const difference_type offset) noexcept {
        BaseClass::operator-=(offset);
        return *this;
    }

    using BaseClass::operator-;

      base_constexpr_cxx20 StringIterator operator-(const difference_type offset) const noexcept {
        StringIterator temp = *this;
        temp -= offset;

        return temp;
    }

      base_constexpr_cxx20 reference operator[](const difference_type offset) const noexcept {
        return const_cast<reference>(BaseClass::operator[](offset));
    }
};

__BASE_STRING_NAMESPACE_END
