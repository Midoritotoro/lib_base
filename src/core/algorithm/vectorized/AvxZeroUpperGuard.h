#pragma once 

#include <base/core/arch/SimdHelpers.h>


__BASE_ALGORITHM_NAMESPACE_BEGIN

struct ZeroUpperOnDeleteAvx {
    ZeroUpperOnDeleteAvx() = default;

    ZeroUpperOnDeleteAvx(const ZeroUpperOnDeleteAvx&) = delete;
    ZeroUpperOnDeleteAvx& operator=(const ZeroUpperOnDeleteAvx&) = delete;

    ~ZeroUpperOnDeleteAvx() {
        _mm256_zeroupper();
    }
};

__BASE_ALGORITHM_NAMESPACE_END
