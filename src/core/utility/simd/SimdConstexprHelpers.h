#pragma once 

#include <base/core/arch/Platform.h>

__BASE_NAMESPACE_BEGIN


constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epi8(int8 value) noexcept { 
#if defined(CPP_MSVC)
    return base_vec128i_t.m128_i8; // ...
#elif defined(CPP_CLANG) || defined(CPP_GNU)

#endif
}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epi16(int16 value) noexcept { 

}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epi32(int32 value) noexcept { 

}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epi64(int64 value) noexcept { 

}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epu8(uint8 value) noexcept { 

}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epu16(uint16 value) noexcept { 

}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epu32(uint32 value) noexcept { 

}

constexpr NODISCARD base_vec128i_t _base_constexpr_mm_set1_epu64(uint64 value) noexcept { 

}


__BASE_NAMESPACE_END

