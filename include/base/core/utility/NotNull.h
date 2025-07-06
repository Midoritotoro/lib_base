#pragma once 

#include <cstddef>      // for ptrdiff_t, nullptr_t, size_t
#include <functional>   // for less, greater

#include <memory>       // for shared_ptr, unique_ptr, hash

#include <utility>      // for declval, forward
#include <iosfwd>       // for ostream

#include <src/core/utility/IsComparableToNullptr.h>
#include <base/core/utility/Assert.h>

__BASE_NAMESPACE_BEGIN

template <
    class _Type_,
    std::enable_if_t<std::is_pointer<_Type_>::value, bool> = true>
using owner = _Type_;

//
// not_null
//
// Restricts a pointer or smart pointer to only hold non-null values.
//
// Has zero size overhead over T.
//
// If T is a pointer (i.e. T == U*) then
// - allow construction from U*
// - disallow construction from nullptr_t
// - disallow default construction
// - ensure construction from null U* fails
// - allow implicit conversion to U*
//
template <class _Type_>
class not_null
{
public:
    static_assert(is_comparable_to_nullptr<_Type_>::value, "_Type_ cannot be compared to nullptr.");

    using element_type = _Type_;

    template <
        typename U, 
        typename = std::enable_if_t<std::is_convertible<U, _Type_>::value>>
    constexpr not_null(U&& u) noexcept(std::is_nothrow_move_constructible<_Type_>::value):
        _pointer(std::forward<U>(u))
    {
        Expects(_pointer != nullptr);
    }

    template <typename = std::enable_if_t<!std::is_same<std::nullptr_t, _Type_>::value>>
    constexpr not_null(_Type_ u) noexcept(std::is_nothrow_move_constructible<_Type_>::value):
        _pointer(std::move(u))
    {
        Expects(_pointer != nullptr);
    }

    template <
        typename U, 
        typename = std::enable_if_t<std::is_convertible<U, _Type_>::value>>
    constexpr not_null(const not_null<U>& other) noexcept(std::is_nothrow_move_constructible<_Type_>::value):
        not_null(other.get())
    {}

    not_null(const not_null& other) = default;
    not_null& operator=(const not_null& other) = default;

    constexpr value_or_reference_return_t<_Type_> get() const
        noexcept(noexcept(value_or_reference_return_t<_Type_>(std::declval<_Type_&>())))
    {
        return _pointer;
    }

    constexpr operator _Type_() const { 
        return get();
    }
    
    constexpr decltype(auto) operator->() const {
        return get(); 
    }

    constexpr decltype(auto) operator*() const { 
        return *get();
    }

    // prevents compilation when someone attempts to assign a null pointer constant
    not_null(std::nullptr_t) = delete;
    not_null& operator=(std::nullptr_t) = delete;

    // unwanted operators...pointers only point to single objects!
    not_null& operator++() = delete;
    not_null& operator--() = delete;

    not_null operator++(int) = delete;
    not_null operator--(int) = delete;

    not_null& operator+=(std::ptrdiff_t) = delete;
    not_null& operator-=(std::ptrdiff_t) = delete;

    void operator[](std::ptrdiff_t) const = delete;

    void swap(not_null<_Type_>& other) {
        std::swap(_pointer, other._pointer);
    }
private:
    _Type_ _pointer;
};

template <
    typename _Type_,
    std::enable_if_t<std::is_move_assignable<_Type_>::value&& std::is_move_constructible<_Type_>::value, bool> = true>
void swap(
    not_null<_Type_>& a,
    not_null<_Type_>& b)
{
    a.swap(b);
}

template <class _Type_>
auto make_not_null(_Type_&& t) noexcept
{
    return not_null<
        std::remove_cv_t<
            std::remove_reference_t<_Type_>>>{std::forward<_Type_>(t)};
}

template <class _Type_>
std::ostream& operator<<(
    std::ostream& os,
    const not_null<_Type_>& value)
{
    os << value.get();
    return os;
}

template <
    class _FirstType_,
    class _SecondType_>
constexpr auto operator==(
    const not_null<_FirstType_>&  lhs,
    const not_null<_SecondType_>& rhs) noexcept(noexcept(lhs.get() == rhs.get()))
        -> decltype(lhs.get() == rhs.get())
{
    return lhs.get() == rhs.get();
}

template <
    class _FirstType_,
    class _SecondType_>
constexpr auto operator!=(
    const not_null<_FirstType_>& lhs,
    const not_null<_SecondType_>& rhs) noexcept(noexcept(lhs.get() != rhs.get()))
        -> decltype(lhs.get() != rhs.get())
{
    return lhs.get() != rhs.get();
}

template <
    class _FirstType_,
    class _SecondType_>
constexpr auto operator<(
    const not_null<_FirstType_>& lhs,
    const not_null<_SecondType_>& rhs) noexcept(noexcept(std::less<>{}(lhs.get(), rhs.get())))
        -> decltype(std::less<>{}(lhs.get(), rhs.get()))
{
    return std::less<>{}(lhs.get(), rhs.get());
}

template <
    class _FirstType_,
    class _SecondType_>
constexpr auto operator<=(
    const not_null<_FirstType_>& lhs,
    const not_null<_SecondType_>& rhs) noexcept(noexcept(std::less_equal<>{}(lhs.get(), rhs.get())))
        -> decltype(std::less_equal<>{}(lhs.get(), rhs.get()))
{
    return std::less_equal<>{}(lhs.get(), rhs.get());
}

template <
    class _FirstType_,
    class _SecondType_>
constexpr auto operator>(
    const not_null<_FirstType_>& lhs,
    const not_null<_SecondType_>& rhs) noexcept(noexcept(std::greater<>{}(lhs.get(), rhs.get())))
        -> decltype(std::greater<>{}(lhs.get(), rhs.get()))
{
    return std::greater<>{}(lhs.get(), rhs.get());
}

template <
    class _FirstType_,
    class _SecondType_>
constexpr auto operator>=(
    const not_null<_FirstType_>& lhs,
    const not_null<_SecondType_>& rhs) noexcept(noexcept(std::greater_equal<>{}(lhs.get(), rhs.get())))
        -> decltype(std::greater_equal<>{}(lhs.get(), rhs.get()))
{
    return std::greater_equal<>{}(lhs.get(), rhs.get());
}

// more unwanted operators
template <
    class _FirstType_,
    class _SecondType_>
std::ptrdiff_t operator-(
    const not_null<_FirstType_>&,
    const not_null<_SecondType_>&) = delete;

template <class _Type_>
not_null<_Type_> operator-(
    const not_null<_Type_>&,
    std::ptrdiff_t
) = delete;

template <class _Type_>
not_null<_Type_> operator+(
    const not_null<_Type_>&,
    std::ptrdiff_t
) = delete;

template <class _Type_>
not_null<_Type_> operator+(
    std::ptrdiff_t, 
    const not_null<_Type_>&
) = delete;

template <
    class _FirstType_,
    class _SecondType_ = decltype(std::declval<
        const _FirstType_&>().get()), 
        bool = std::is_default_constructible<std::hash<_SecondType_>>::value>
struct not_null_hash
{
    std::size_t operator()(const _FirstType_& value) const {
        return std::hash<_SecondType_>{}(value.get()); 
    }
};

template <
    class _FirstType_, 
    class _SecondType_>
struct not_null_hash<_FirstType_, _SecondType_, false>
{
    not_null_hash() = delete;
    not_null_hash(const not_null_hash&) = delete;
    not_null_hash& operator=(const not_null_hash&) = delete;
};

#if (defined(__cpp_deduction_guides) && (__cpp_deduction_guides >= 201611L))

// deduction guides to prevent the ctad-maybe-unsupported warning
template <class T>
not_null(T) -> not_null<T>;

#endif // ( defined(__cpp_deduction_guides) && (__cpp_deduction_guides >= 201611L) )

__BASE_NAMESPACE_END

namespace std
{
   template <class _Type_>
   struct hash<base::not_null<_Type_>>: 
       base::not_null_hash<base::not_null<_Type_>>
   {};

} // namespace std

