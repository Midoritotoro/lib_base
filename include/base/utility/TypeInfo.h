#pragma once 

#include <base/TypeTraits.h>

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