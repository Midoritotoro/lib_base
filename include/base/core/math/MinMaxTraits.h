#pragma once

#ifndef _DEBUG
#  pragma optimize("t", on)
#endif // !defined(_DEBUG)

#include <base/core/math/MinMaxTraitsSSE2.h>

#include <base/core/math/MinMaxTraitsAVX.h>
#include <base/core/math/MinMaxTraitsAVX512.h>

__BASE_NAMESPACE_BEGIN

template <typename _Integer_>
static inline constexpr void FillArray(
    _Integer_* _Array,
    sizetype _Size,
    _Integer_ _Val)
{
    for (sizetype i = 0; i < _Size; ++i)
        _Array[i] = _Val;
}

template <typename _Type_>
concept IsValidIntegerType = std::is_integral_v<_Type_>;
    //&& sizeof(_Type_) >= 1 
    //&& sizeof(_Type_) <= 8;

namespace Scalar {
    template <
        typename Integer,
        typename = std::enable_if_t<IsValidIntegerType<Integer>>>
    bool GetMinimum(
        const Integer*  _Start,
        const Integer*  _End,
        Integer*        _Out)
    {
        const auto _Length = memory::ByteLength(_Start, _End);

        sizetype   _Temp = 0;
        sizetype   _TempMin = _Temp;

        if (memory::MemoryCopy(memory::AdressOf(&_Temp), _Start, sizeof(Integer)) == false)
            return false;

        for (sizetype i = 0; i < _Length; ++i) {
            if (memory::MemoryCopy(memory::AdressOf(&_TempMin), _Start + i, sizeof(Integer)) == false)
                return false;

            if (_TempMin < _Temp)
                _Temp = _TempMin;
        }

        *_Out = _Temp;
        return true;
    }

    template <
        typename Integer,
        typename = std::enable_if_t<IsValidIntegerType<Integer>>>
    bool GetMaximum(
        const Integer*  _Start,
        const Integer*  _End,
        Integer*        _Out)
    {
        const auto _Length = memory::ByteLength(_Start, _End);

        sizetype   _Temp = 0;
        sizetype   _TempMax = _Temp;

        if (memory::MemoryCopy(memory::AdressOf(&_Temp), _Start, sizeof(Integer)) == false)
            return false;

        for (sizetype i = 0; i < _Length; ++i) {
            if (memory::MemoryCopy(memory::AdressOf(&_TempMax), _Start + i, sizeof(Integer)) == false)
                return false;

            if (_TempMax > _Temp)
                _Temp = _TempMax;
        }

        *_Out = _Temp;
        return true;
    }
} // namespace Scalar

__BASE_NAMESPACE_END