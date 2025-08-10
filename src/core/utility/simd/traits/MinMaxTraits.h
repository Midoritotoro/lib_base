#pragma once

#ifndef _DEBUG
#  pragma optimize("t", on)
#endif // !defined(_DEBUG)

#include <base/core/utility/TypeTraits.h>
#include <base/core/arch/SimdHelpers.h>

__BASE_NAMESPACE_BEGIN


namespace Scalar {
    template <
        typename Integer,
        typename = std::enable_if_t<std::is_integral_v<Integer>>>
    void GetMinimum(
        const void*     _Start,
        const void*     _End,
        Integer*        _Out)
    {
        Integer _Temp = 0;

        for (auto _Ptr = static_cast<const Integer*>(_Start); _Ptr != _End; ++_Ptr)
            if (*_Ptr < _Temp)
                _Temp = *_Ptr;

        *_Out = _Temp;
    }

    template <
        typename Integer,
        typename = std::enable_if_t<std::is_integral_v<Integer>>>
    void GetMaximum(
        const void*     _Start,
        const void*     _End,
        Integer*        _Out)
    {
        Integer _Temp = 0;

        for (auto _Ptr = static_cast<const Integer*>(_Start); _Ptr != _End; ++_Ptr)
            if (*_Ptr > _Temp)
                _Temp = *_Ptr;

        *_Out = _Temp;
    }
} // namespace Scalar

__BASE_NAMESPACE_END