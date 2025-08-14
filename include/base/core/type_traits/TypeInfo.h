#pragma once 

#include <base/core/type_traits/TypeCheck.h>
#include <base/core/utility/Flags.h>


template <typename T>
class TypeInfo
{
public:
    enum {
        isPointer [[deprecated("Use std::is_pointer instead")]] = std::is_pointer_v<T>,
        isIntegral [[deprecated("Use std::is_integral instead")]] = std::is_integral_v<T>,
        isComplex = !std::is_trivial_v<T>,
        isRelocatable = base::type_traits::is_relocatable_v<T>,
        isValueInitializationBitwiseZero = base::type_traits::is_value_initialization_bitwise_zero_v<T>,
    };
};

template<>
class TypeInfo<void>
{
public:
    enum {
        isPointer [[deprecated("Use std::is_pointer instead")]] = false,
        isIntegral [[deprecated("Use std::is_integral instead")]] = false,
        isComplex = false,
        isRelocatable = false,
        isValueInitializationBitwiseZero = false,
    };
};

#define DECLARE_MOVABLE_CONTAINER(CONTAINER) \
template <typename ...T> \
class TypeInfo<CONTAINER<T...>> \
{ \
public: \
    enum { \
        isPointer [[deprecated("Use std::is_pointer instead")]] = false, \
        isIntegral [[deprecated("Use std::is_integral instead")]] = false, \
        isComplex = true, \
        isRelocatable = true, \
        isValueInitializationBitwiseZero = false, \
    }; \
}
enum { /* TYPEINFO flags */
    COMPLEX_TYPE = 0,
    PRIMITIVE_TYPE = 0x1,
    RELOCATABLE_TYPE = 0x2,
    MOVABLE_TYPE = 0x2,
    DUMMY_TYPE = 0x4,
};

#define DECLARE_TYPEINFO_BODY(TYPE, FLAGS) \
class TypeInfo<TYPE > \
{ \
public: \
    enum { \
        isComplex = (((FLAGS) & PRIMITIVE_TYPE) == 0) && !std::is_trivial_v<TYPE>, \
        isRelocatable = !isComplex || ((FLAGS) & RELOCATABLE_TYPE) || base::type_traits::is_relocatable_v<TYPE>, \
        isPointer [[deprecated("Use std::is_pointer instead")]] = std::is_pointer_v< TYPE >, \
        isIntegral [[deprecated("Use std::is_integral instead")]] = std::is_integral< TYPE >::value, \
        isValueInitializationBitwiseZero = base::type_traits::is_value_initialization_bitwise_zero_v<TYPE>, \
    }; \
    static_assert(!isRelocatable || \
                  std::is_copy_constructible_v<TYPE > || \
                  std::is_move_constructible_v<TYPE >, \
                  #TYPE " is neither copy- nor move-constructible, so cannot be Q_RELOCATABLE_TYPE"); \
}

#define DECLARE_TYPEINFO(TYPE, FLAGS) \
template<> \
DECLARE_TYPEINFO_BODY(TYPE, FLAGS)

#if defined(base_cpp_msvc)
template<typename T> class base::flags;
template<typename T>
#endif
DECLARE_TYPEINFO_BODY(base::flags<T>, PRIMITIVE_TYPE);
