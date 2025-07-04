#pragma once 

#include <cstddef>      // for ptrdiff_t, nullptr_t, size_t
#include <functional>   // for less, greater
#include <memory>       // for shared_ptr, unique_ptr, hash
#include <type_traits>  // for enable_if_t, is_convertible, is_assignable
#include <utility>      // for declval, forward
#include <iosfwd>       // for ostream

namespace gsl
{

   namespace details
   {
       template <typename T, typename = void>
       struct is_comparable_to_nullptr : std::false_type
       {
       };

       template <typename T>
       struct is_comparable_to_nullptr<
           T,
           std::enable_if_t<std::is_convertible<decltype(std::declval<T>() != nullptr), bool>::value>>
           : std::true_type
       {
       };

       // Resolves to the more efficient of `const T` or `const T&`, in the context of returning a const-qualified value
       // of type T.
       //
       // Copied from cppfront's implementation of the CppCoreGuidelines F.16 (https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rf-in)
       template<typename T>
       using value_or_reference_return_t = std::conditional_t<
           sizeof(T) <= 2 * sizeof(void*) && std::is_trivially_copy_constructible<T>::value,
           const T,
           const T&>;

   } // namespace details

   //
   // GSL.owner: ownership pointers
   //
   template <typename... Ts>
   using shared_ptr GSL_DEPRECATED("Use std::shared_ptr instead") = std::shared_ptr<Ts...>;

   template <typename... Ts>
   using unique_ptr GSL_DEPRECATED("Use std::unique_ptr instead") = std::unique_ptr<Ts...>;

   //
   // owner
   //
   // `gsl::owner<T>` is designed as a safety mechanism for code that must deal directly with raw pointers that own memory.
   // Ideally such code should be restricted to the implementation of low-level abstractions. `gsl::owner` can also be used
   // as a stepping point in converting legacy code to use more modern RAII constructs, such as smart pointers.
   //
   // T must be a pointer type
   // - disallow construction from any type other than pointer type
   //
   template <class T, std::enable_if_t<std::is_pointer<T>::value, bool> = true>
   using owner = T;

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
   template <class T>
   class not_null
   {
   public:
       static_assert(details::is_comparable_to_nullptr<T>::value, "T cannot be compared to nullptr.");

       using element_type = T;

       template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
       constexpr not_null(U&& u) noexcept(std::is_nothrow_move_constructible<T>::value) : ptr_(std::forward<U>(u))
       {
           Expects(ptr_ != nullptr);
       }

       template <typename = std::enable_if_t<!std::is_same<std::nullptr_t, T>::value>>
       constexpr not_null(T u) noexcept(std::is_nothrow_move_constructible<T>::value) : ptr_(std::move(u))
       {
           Expects(ptr_ != nullptr);
       }

       template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
       constexpr not_null(const not_null<U>& other) noexcept(std::is_nothrow_move_constructible<T>::value) : not_null(other.get())
       {
       }

       not_null(const not_null& other) = default;
       not_null& operator=(const not_null& other) = default;
       constexpr details::value_or_reference_return_t<T> get() const
           noexcept(noexcept(details::value_or_reference_return_t<T>(std::declval<T&>())))
       {
           return ptr_;
       }

       constexpr operator T() const { return get(); }
       constexpr decltype(auto) operator->() const { return get(); }
       constexpr decltype(auto) operator*() const { return *get(); }

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

       void swap(not_null<T>& other) { std::swap(ptr_, other.ptr_); }

   private:
       T ptr_;
   };

   template <typename T, std::enable_if_t<std::is_move_assignable<T>::value&& std::is_move_constructible<T>::value, bool> = true>
   void swap(not_null<T>& a, not_null<T>& b)
   {
       a.swap(b);
   }

   template <class T>
   auto make_not_null(T&& t) noexcept
   {
       return not_null<std::remove_cv_t<std::remove_reference_t<T>>>{std::forward<T>(t)};
   }

#if !defined(GSL_NO_IOSTREAMS)
   template <class T>
   std::ostream& operator<<(std::ostream& os, const not_null<T>& val)
   {
       os << val.get();
       return os;
   }
#endif // !defined(GSL_NO_IOSTREAMS)

   template <class T, class U>
   constexpr auto operator==(const not_null<T>& lhs,
       const not_null<U>& rhs) noexcept(noexcept(lhs.get() == rhs.get()))
       -> decltype(lhs.get() == rhs.get())
   {
       return lhs.get() == rhs.get();
   }

   template <class T, class U>
   constexpr auto operator!=(const not_null<T>& lhs,
       const not_null<U>& rhs) noexcept(noexcept(lhs.get() != rhs.get()))
       -> decltype(lhs.get() != rhs.get())
   {
       return lhs.get() != rhs.get();
   }

   template <class T, class U>
   constexpr auto operator<(const not_null<T>& lhs,
       const not_null<U>& rhs) noexcept(noexcept(std::less<>{}(lhs.get(), rhs.get())))
       -> decltype(std::less<>{}(lhs.get(), rhs.get()))
   {
       return std::less<>{}(lhs.get(), rhs.get());
   }

   template <class T, class U>
   constexpr auto operator<=(const not_null<T>& lhs,
       const not_null<U>& rhs) noexcept(noexcept(std::less_equal<>{}(lhs.get(), rhs.get())))
       -> decltype(std::less_equal<>{}(lhs.get(), rhs.get()))
   {
       return std::less_equal<>{}(lhs.get(), rhs.get());
   }

   template <class T, class U>
   constexpr auto operator>(const not_null<T>& lhs,
       const not_null<U>& rhs) noexcept(noexcept(std::greater<>{}(lhs.get(), rhs.get())))
       -> decltype(std::greater<>{}(lhs.get(), rhs.get()))
   {
       return std::greater<>{}(lhs.get(), rhs.get());
   }

   template <class T, class U>
   constexpr auto operator>=(const not_null<T>& lhs,
       const not_null<U>& rhs) noexcept(noexcept(std::greater_equal<>{}(lhs.get(), rhs.get())))
       -> decltype(std::greater_equal<>{}(lhs.get(), rhs.get()))
   {
       return std::greater_equal<>{}(lhs.get(), rhs.get());
   }

   // more unwanted operators
   template <class T, class U>
   std::ptrdiff_t operator-(const not_null<T>&, const not_null<U>&) = delete;
   template <class T>
   not_null<T> operator-(const not_null<T>&, std::ptrdiff_t) = delete;
   template <class T>
   not_null<T> operator+(const not_null<T>&, std::ptrdiff_t) = delete;
   template <class T>
   not_null<T> operator+(std::ptrdiff_t, const not_null<T>&) = delete;


   template <class T, class U = decltype(std::declval<const T&>().get()), bool = std::is_default_constructible<std::hash<U>>::value>
   struct not_null_hash
   {
       std::size_t operator()(const T& value) const { return std::hash<U>{}(value.get()); }
   };

   template <class T, class U>
   struct not_null_hash<T, U, false>
   {
       not_null_hash() = delete;
       not_null_hash(const not_null_hash&) = delete;
       not_null_hash& operator=(const not_null_hash&) = delete;
   };

} // namespace gsl

namespace std
{
   template <class T>
   struct hash<gsl::not_null<T>> : gsl::not_null_hash<gsl::not_null<T>>
   {
   };

} // namespace std

namespace gsl
{

   //
   // strict_not_null
   //
   // Restricts a pointer or smart pointer to only hold non-null values,
   //
   // - provides a strict (i.e. explicit constructor from T) wrapper of not_null
   // - to be used for new code that wishes the design to be cleaner and make not_null
   //   checks intentional, or in old code that would like to make the transition.
   //
   //   To make the transition from not_null, incrementally replace not_null
   //   by strict_not_null and fix compilation errors
   //
   //   Expect to
   //   - remove all unneeded conversions from raw pointer to not_null and back
   //   - make API clear by specifying not_null in parameters where needed
   //   - remove unnecessary asserts
   //
   template <class T>
   class strict_not_null : public not_null<T>
   {
   public:
       template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
       constexpr explicit strict_not_null(U&& u) noexcept(std::is_nothrow_move_constructible<T>::value) : not_null<T>(std::forward<U>(u))
       {
       }

       template <typename = std::enable_if_t<!std::is_same<std::nullptr_t, T>::value>>
       constexpr explicit strict_not_null(T u) noexcept(std::is_nothrow_move_constructible<T>::value) : not_null<T>(std::move(u))
       {
       }

       template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
       constexpr strict_not_null(const not_null<U>& other) noexcept(std::is_nothrow_move_constructible<T>::value) : not_null<T>(other)
       {
       }

       template <typename U, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
       constexpr strict_not_null(const strict_not_null<U>& other) noexcept(std::is_nothrow_move_constructible<T>::value) : not_null<T>(other)
       {
       }

       // To avoid invalidating the "not null" invariant, the contained pointer is actually copied
       // instead of moved. If it is a custom pointer, its constructor could in theory throw exceptions.
       strict_not_null(strict_not_null&& other) noexcept(std::is_nothrow_copy_constructible<T>::value) = default;
       strict_not_null(const strict_not_null& other) = default;
       strict_not_null& operator=(const strict_not_null& other) = default;
       strict_not_null& operator=(const not_null<T>& other)
       {
           not_null<T>::operator=(other);
           return *this;
       }

       // prevents compilation when someone attempts to assign a null pointer constant
       strict_not_null(std::nullptr_t) = delete;
       strict_not_null& operator=(std::nullptr_t) = delete;

       // unwanted operators...pointers only point to single objects!
       strict_not_null& operator++() = delete;
       strict_not_null& operator--() = delete;
       strict_not_null operator++(int) = delete;
       strict_not_null operator--(int) = delete;
       strict_not_null& operator+=(std::ptrdiff_t) = delete;
       strict_not_null& operator-=(std::ptrdiff_t) = delete;
       void operator[](std::ptrdiff_t) const = delete;
   };

   // more unwanted operators
   template <class T, class U>
   std::ptrdiff_t operator-(const strict_not_null<T>&, const strict_not_null<U>&) = delete;
   template <class T>
   strict_not_null<T> operator-(const strict_not_null<T>&, std::ptrdiff_t) = delete;
   template <class T>
   strict_not_null<T> operator+(const strict_not_null<T>&, std::ptrdiff_t) = delete;
   template <class T>
   strict_not_null<T> operator+(std::ptrdiff_t, const strict_not_null<T>&) = delete;

   template <class T>
   auto make_strict_not_null(T&& t) noexcept
   {
       return strict_not_null<std::remove_cv_t<std::remove_reference_t<T>>>{std::forward<T>(t)};
   }

#if (defined(__cpp_deduction_guides) && (__cpp_deduction_guides >= 201611L))

   // deduction guides to prevent the ctad-maybe-unsupported warning
   template <class T>
   not_null(T) -> not_null<T>;
   template <class T>
   strict_not_null(T) -> strict_not_null<T>;

#endif // ( defined(__cpp_deduction_guides) && (__cpp_deduction_guides >= 201611L) )

} // namespace gsl

namespace std
{
   template <class T>
   struct hash<gsl::strict_not_null<T>> : gsl::not_null_hash<gsl::strict_not_null<T>>
   {
   };

} // namespace std
