#pragma once 

#include <base/images/Images.h>
#include <base/SimdHelpers.h>


namespace base::images {
    namespace {
#ifdef LIB_BASE_ENABLE_sse2
    void BradleyHelperSse2(
        uchar* src, uchar* res,
        int32 width, int32 height,
        int32 _S, int32 _T);
#endif
    } // namespace

    void BradleyThreshold(
        uchar* src, uchar* res,
        int32 width, int32 height,
        ushort imageChannelsCount);
} // namespace base::images