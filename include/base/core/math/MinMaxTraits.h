#pragma once

#ifndef _DEBUG
#  pragma optimize("t", on)
#endif // !defined(_DEBUG)

#include <base/core/math/MinMaxTraitsSSE2.h>

#include <base/core/math/MinMaxTraitsAVX.h>
#include <base/core/math/MinMaxTraitsAVX512.h>

__BASE_NAMESPACE_BEGIN

template <typename _Type_>
concept IsValidIntegerType = std::is_integral_v<_Type_>;

namespace Scalar {
    template <
        typename Integer,
        typename = std::enable_if_t<IsValidIntegerType<Integer>>>
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
        typename = std::enable_if_t<IsValidIntegerType<Integer>>>
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