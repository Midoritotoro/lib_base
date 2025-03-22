#pragma once 

#include <base/core/arch/Platform.h>

#ifndef _DEBUG
#  pragma optimize("t", on)
#endif // !defined(_DEBUG)


__BASE_CONTAINER_NAMESPACE_BEGIN

struct MinMaxInt8Bit {
    int8_t minimum = 0;
    int8_t maximum = 0;
};

struct MinMaxUInt8Bit {
    uint8_t minimum = 0;
    uint8_t maximum = 0;
};

struct MinMaxInt16Bit {
    int16_t minimum = 0;
    int16_t maximum = 0;
};

struct MinMaxUInt16Bit {
    uint16_t minimum = 0;
    uint16_t maximum = 0;
};

struct MinMaxInt32Bit {
    int32_t minimum = 0;
    int32_t maximum = 0;
};

struct MinMaxUInt32Bit {
    uint32_t minimum = 0;
    uint32_t maximum = 0;
};

struct MinMaxInt64Bit {
    int64_t minimum = 0;
    int64_t maximum = 0;
};

struct MinMaxUInt64Bit {
    uint64_t minimum = 0;
    uint64_t maximum = 0;
};

struct MinMaxFloat {
    float minimum = 0;
    float maximum = 0;
};

struct MinMaxDouble {
    double minimum = 0;
    double maximum = 0;
};

namespace {
    template <class _BidIt>
    void ReverseTail(_BidIt _First, _BidIt _Last) noexcept {
        for (; _First != _Last && _First != --_Last; ++_First) {
            const auto _Temp = *_First;
            *_First = *_Last;
            *_Last = _Temp;
        }
    }

    template <class _BidIt, class _OutIt>
    void ReverseCopyTail(_BidIt _First, _BidIt _Last, _OutIt _Dest) noexcept {
        while (_First != _Last)
            *_Dest++ = *--_Last;
    }

    size_t ByteLength(const void* _First, const void* _Last) noexcept {
        return static_cast<const unsigned char*>(_Last) 
            - static_cast<const unsigned char*>(_First);
    }

    void RewindBytes(void*& _Target, size_t _Offset) noexcept {
        _Target = static_cast<unsigned char*>(_Target) - _Offset;
    }

    void RewindBytes(const void*& _Target, size_t _Offset) noexcept {
        _Target = static_cast<const unsigned char*>(_Target) - _Offset;
    }

    template <class _Integral>
    void AdvanceBytes(void*& _Target, _Integral _Offset) noexcept {
        _Target = static_cast<unsigned char*>(_Target) + _Offset;
    }

    template <class _Integral>
    void AdvanceBytes(const void*& _Target, _Integral _Offset) noexcept {
        _Target = static_cast<const unsigned char*>(_Target) + _Offset;
    }
} // namespace

#if LIB_BASE_HAS_SIMD_SUPPORT


template <typename Integer>
class VectorizedTraits {
    
};

DECLARE_NOALIAS void __CDECL MaxInteger16x8(
	const void* _Start,
	const void* _End) noexcept
{
	
}

#endif // LIB_BASE_HAS_SIMD_SUPPORT

__BASE_CONTAINER_NAMESPACE_END