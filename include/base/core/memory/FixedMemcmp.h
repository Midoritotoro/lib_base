#pragma once 

#include <base/core/arch/Platform.h>

__BASE_MEMORY_NAMESPACE_BEGIN

DECLARE_NOALIAS NODISCARD always_inline bool alwaysTrue(const char*, const char*) noexcept {
    return true;
}


DECLARE_NOALIAS NODISCARD always_inline bool memcmp1(const char* a, const char* b) noexcept {
    return a[0] == b[0];
}


DECLARE_NOALIAS NODISCARD always_inline bool memcmp2(const char* a, const char* b) noexcept {
    const uint16_t A = *reinterpret_cast<const uint16_t*>(a);
    const uint16_t B = *reinterpret_cast<const uint16_t*>(b);

    return A == B;
}


DECLARE_NOALIAS NODISCARD always_inline bool memcmp3(const char* a, const char* b) noexcept {
    const uint32_t A = *reinterpret_cast<const uint32_t*>(a);
    const uint32_t B = *reinterpret_cast<const uint32_t*>(b);

    return (A & 0x00ffffff) == (B & 0x00ffffff);
}


DECLARE_NOALIAS NODISCARD always_inline bool memcmp4(const char* a, const char* b) noexcept {
    const uint32_t A = *reinterpret_cast<const uint32_t*>(a);
    const uint32_t B = *reinterpret_cast<const uint32_t*>(b);

    return A == B;
}


DECLARE_NOALIAS NODISCARD always_inline bool memcmp5(const char* a, const char* b) noexcept {
    const uint64_t A = *reinterpret_cast<const uint64_t*>(a);
    const uint64_t B = *reinterpret_cast<const uint64_t*>(b);

    return ((A ^ B) & 0x000000fffffffffflu) == 0;
}


DECLARE_NOALIAS NODISCARD always_inline bool memcmp6(const char* a, const char* b) noexcept {
    const uint64_t A = *reinterpret_cast<const uint64_t*>(a);
    const uint64_t B = *reinterpret_cast<const uint64_t*>(b);

    return ((A ^ B) & 0x0000fffffffffffflu) == 0;
}


DECLARE_NOALIAS NODISCARD always_inline bool memcmp7(const char* a, const char* b) noexcept {
    const uint64_t A = *reinterpret_cast<const uint64_t*>(a);
    const uint64_t B = *reinterpret_cast<const uint64_t*>(b);

    return ((A ^ B) & 0x00fffffffffffffflu) == 0;
}


DECLARE_NOALIAS NODISCARD always_inline bool memcmp8(const char* a, const char* b) noexcept {
    const uint64_t A = *reinterpret_cast<const uint64_t*>(a);
    const uint64_t B = *reinterpret_cast<const uint64_t*>(b);

    return A == B;
}


DECLARE_NOALIAS NODISCARD always_inline bool memcmp9(const char* a, const char* b) noexcept {

    const uint64_t A = *reinterpret_cast<const uint64_t*>(a);
    const uint64_t B = *reinterpret_cast<const uint64_t*>(b);

    return (A == B) & (a[8] == b[8]);
}


DECLARE_NOALIAS NODISCARD always_inline bool memcmp10(const char* a, const char* b) noexcept {
    const uint64_t Aq = *reinterpret_cast<const uint64_t*>(a);
    const uint64_t Bq = *reinterpret_cast<const uint64_t*>(b);

    const uint16_t Aw = *reinterpret_cast<const uint16_t*>(a + 8);
    const uint16_t Bw = *reinterpret_cast<const uint16_t*>(b + 8);

    return (Aq == Bq) & (Aw == Bw);
}


DECLARE_NOALIAS NODISCARD always_inline bool memcmp11(const char* a, const char* b) noexcept {
    const uint64_t Aq = *reinterpret_cast<const uint64_t*>(a);
    const uint64_t Bq = *reinterpret_cast<const uint64_t*>(b);

    const uint32_t Ad = *reinterpret_cast<const uint32_t*>(a + 8);
    const uint32_t Bd = *reinterpret_cast<const uint32_t*>(b + 8);

    return (Aq == Bq) & ((Ad & 0x00ffffff) == (Bd & 0x00ffffff));
}


DECLARE_NOALIAS NODISCARD always_inline bool memcmp12(const char* a, const char* b) noexcept {
    const uint64_t Aq = *reinterpret_cast<const uint64_t*>(a);
    const uint64_t Bq = *reinterpret_cast<const uint64_t*>(b);

    const uint32_t Ad = *reinterpret_cast<const uint32_t*>(a + 8);
    const uint32_t Bd = *reinterpret_cast<const uint32_t*>(b + 8);

    return (Aq == Bq) & (Ad == Bd);
}

__BASE_MEMORY_NAMESPACE_END
