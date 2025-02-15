#pragma once 

#include <base/utility/TypeTraits.h>
#include <base/utility/Flags.h>


namespace base {
    template <typename T>
    class TypeInfo
    {
    public:
        enum {
            isPointer [[deprecated("Use std::is_pointer instead")]] = std::is_pointer_v<T>,
            isIntegral [[deprecated("Use std::is_integral instead")]] = std::is_integral_v<T>,
            isComplex = !std::is_trivial_v<T>,
            isRelocatable = IsRelocatable<T>,
            isValueInitializationBitwiseZero = IsValueInitializationBitwiseZero<T>,
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
} // namespace base

enum { /* TYPEINFO flags */
    COMPLEX_TYPE = 0,
    PRIMITIVE_TYPE = 0x1,
    RELOCATABLE_TYPE = 0x2,
    MOVABLE_TYPE = 0x2,
    DUMMY_TYPE = 0x4,
};

#define DECLARE_TYPEINFO_BODY(TYPE, FLAGS) \
class base::TypeInfo<TYPE > \
{ \
public: \
    enum { \
        isComplex = (((FLAGS) & PRIMITIVE_TYPE) == 0) && !std::is_trivial_v<TYPE>, \
        isRelocatable = !isComplex || ((FLAGS) & RELOCATABLE_TYPE) || base::IsRelocatable<TYPE>, \
        isPointer [[deprecated("Use std::is_pointer instead")]] = std::is_pointer_v< TYPE >, \
        isIntegral [[deprecated("Use std::is_integral instead")]] = std::is_integral< TYPE >::value, \
        isValueInitializationBitwiseZero = base::IsValueInitializationBitwiseZero<TYPE>, \
    }; \
    static_assert(!isRelocatable || \
        std::is_copy_constructible_v<TYPE > || \
        std::is_move_constructible_v<TYPE >, \
        #TYPE " is neither copy- nor move-constructible, so cannot be RELOCATABLE_TYPE"); \
}

#define DECLARE_TYPEINFO(TYPE, FLAGS) \
template<> \
DECLARE_TYPEINFO_BODY(TYPE, FLAGS)


template<typename T> class base::flags;
template<typename T>
DECLARE_TYPEINFO_BODY(base::flags<T>, PRIMITIVE_TYPE);