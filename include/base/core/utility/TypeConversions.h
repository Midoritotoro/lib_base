#pragma once 


#include <base/core/utility/TypeTraits.h> 

__BASE_NAMESPACE_BEGIN


template <bool>
struct TypeSelect { 
    // Select between aliases that extract either their first or second parameter
    template <class _Ty1, class>
    using Apply = _Ty1;
};

template <>
struct TypeSelect<false> {
    template <class, class _Ty2>
    using Apply = _Ty2;
};


template <size_t>
struct MakeSigned2; // Choose make_signed strategy by type size

template <>
struct MakeSigned2<1> {
    template <class>
    using Apply = signed char;
};

template <>
struct MakeSigned2<2> {
    template <class>
    using Apply = short;
};

template <>
struct MakeSigned2<4> {
    template <class _Ty>
    using Apply = // assumes LLP64
        typename TypeSelect<std::is_same_v<_Ty, long> || std::is_same_v<_Ty, unsigned long>>::template Apply<long, int>;
};

template <>
struct MakeSigned2<8> {
    template <class>
    using Apply = long long;
};

template <class _Ty>
using MakeSigned1 = // signed partner to cv-unqualified _Ty
typename MakeSigned2<sizeof(_Ty)>::template Apply<_Ty>;

template <class _Ty>
struct make_signed { // signed partner to _Ty
    static_assert(is_nonbool_integral_v<_Ty> || std::is_enum_v<_Ty>,
        "make_signed<T> requires that T shall be a (possibly cv-qualified) "
        "integral type or enumeration but not a bool type.");

    using type = typename std::remove_cv<_Ty>::template Apply<MakeSigned1>;
};

template <class _Ty>
using make_signed_t = typename make_signed<_Ty>::type;

template <size_t>
struct MakeUnsigned2; // Choose make_unsigned strategy by type size

template <>
struct MakeUnsigned2<1> {
    template <class>
    using Apply = unsigned char;
};

template <>
struct MakeUnsigned2<2> {
    template <class>
    using Apply = unsigned short;
};

template <>
struct MakeUnsigned2<4> {
    template <class _Ty>
    using Apply = // assumes LLP64
        typename SelectType<std::is_same_v<_Ty, long> || std::is_same_v<_Ty, unsigned long>>::template Apply<unsigned long,
        unsigned int>;
};

template <>
struct MakeUnsigned2<8> {
    template <class>
    using Apply = unsigned long long;
};

template <class _Ty>
using MakeUnsigned1 = // unsigned partner to cv-unqualified _Ty
typename MakeUnsigned2<sizeof(_Ty)>::template Apply<_Ty>;

template <class _Ty>
struct make_unsigned { // unsigned partner to _Ty
    static_assert(is_nonbool_integral_v<_Ty> || std::is_enum_v<_Ty>,
        "make_unsigned<T> requires that T shall be a (possibly cv-qualified) "
        "integral type or enumeration but not a bool type.");

    using type = typename std::remove_cv<_Ty>::template Apply<MakeUnsigned1>;
};

template <class _Ty>
using make_unsigned_t = typename make_unsigned<_Ty>::type;

__BASE_NAMESPACE_END