#pragma once 

#include <base/core/container/VectorConstIterator.h> 

__BASE_CONTAINER_NAMESPACE_BEGIN

template <class _Vector>
class VectorIterator :
    public VectorConstIterator<_Vector>
{
public:
    using BaseClass         = VectorConstIterator<_Vector>;
    using iterator_category = std::random_access_iterator_tag;

    using value_type        = typename BaseClass::value_type;
    using difference_type   = sizetype;

    using pointer           = value_type*;
    using size_type         = sizetype;

    using reference         = value_type&;
    using const_reference   = const value_type&;

    using const_pointer     = const pointer*;

    using BaseClass::BaseClass;


      base_constexpr_cxx20 reference operator*() const noexcept {
        return const_cast<reference>(BaseClass::operator*());
    }

    base_constexpr_cxx20 pointer operator->() const noexcept {
        return const_cast<pointer>(BaseClass::operator->());
    }


    base_constexpr_cxx20 VectorIterator& operator++() noexcept {
        BaseClass::operator++();
        return *this;
    }

    base_constexpr_cxx20 VectorIterator operator++(int) noexcept {
        VectorIterator temp = *this;
        BaseClass::operator++();

        return temp;
    }

    base_constexpr_cxx20 VectorIterator& operator--() noexcept {
        BaseClass::operator--();
        return *this;
    }

    base_constexpr_cxx20 VectorIterator operator--(int) noexcept {
        VectorIterator temp = *this;
        BaseClass::operator--();

        return temp;
    }

    base_constexpr_cxx20 VectorIterator& operator+=(const difference_type offset) noexcept {
        BaseClass::operator+=(offset);
        return *this;
    }

      base_constexpr_cxx20 VectorIterator operator+(const difference_type offset) const noexcept {
        VectorIterator temp = *this;
        temp += offset;

        return temp;
    }

      friend base_constexpr_cxx20 VectorIterator operator+(
        const difference_type offset,
        VectorIterator next) noexcept
    {
        next += offset;
        return next;
    }

    base_constexpr_cxx20 VectorIterator& operator-=(const difference_type offset) noexcept {
        BaseClass::operator-=(offset);
        return *this;
    }

    using BaseClass::operator-;

      base_constexpr_cxx20 VectorIterator operator-(const difference_type offset) const noexcept {
        VectorIterator temp = *this;
        temp -= offset;

        return temp;
    }

      base_constexpr_cxx20 reference operator[](const difference_type offset) const noexcept {
        return const_cast<reference>(BaseClass::operator[](offset));
    }
};


__BASE_CONTAINER_NAMESPACE_END