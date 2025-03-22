#pragma once 

#include <base/core/arch/Platform.h>

#ifndef _DEBUG
#  pragma optimize("t", on)
#endif // !defined(_DEBUG)


__BASE_CONTAINER_NAMESPACE_BEGIN

struct min_max_t {
    int8_t minimum;
    int8_t maximum;
};

struct _Min_max_1u {
    uint8_t _Min;
    uint8_t _Max;
};

struct _Min_max_2i {
    int16_t _Min;
    int16_t _Max;
};

struct _Min_max_2u {
    uint16_t _Min;
    uint16_t _Max;
};

struct _Min_max_4i {
    int32_t _Min;
    int32_t _Max;
};

struct _Min_max_4u {
    uint32_t _Min;
    uint32_t _Max;
};

struct _Min_max_8i {
    int64_t _Min;
    int64_t _Max;
};

struct _Min_max_8u {
    uint64_t _Min;
    uint64_t _Max;
};

struct _Min_max_f {
    float _Min;
    float _Max;
};

struct _Min_max_d {
    double _Min;
    double _Max;
};

struct _Min_max_p {
    void* _Min;
    void* _Max;
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
	const auto _Loaded = _mm_loadu_epi16(_Start);
}

#endif // LIB_BASE_HAS_SIMD_SUPPORT

__BASE_CONTAINER_NAMESPACE_END