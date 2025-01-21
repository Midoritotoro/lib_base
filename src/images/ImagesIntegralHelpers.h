#pragma once 

#include <base/images/Images.h>
#include <base/SimdHelpers.h>


namespace base::images {
    namespace {
#ifdef LIB_BASE_ENABLE_avx2
    void BradleyHelperAvx2(
        uchar* src, uchar* res,
        int32 width, int32 height);
#endif
#ifdef LIB_BASE_ENABLE_sse4_2
    void BradleyHelperSse4_2(
        uchar* src, uchar* res,
        int32 width, int32 height);
#endif
#ifdef LIB_BASE_ENABLE_sse3
    void BradleyHelperSse3(
        uchar* src, uchar* res,
        int32 width, int32 height);
#endif
    } // namespace

    void BradleyThreshold(
        uchar* src, uchar* res,
        int32 width, int32 height);
} // namespace base::images