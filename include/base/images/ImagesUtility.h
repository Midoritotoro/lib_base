#pragma once 


#include <base/images/Images.h>


namespace base::images {
#ifdef LIB_BASE_ENABLE_QT 
    int getChannelsCountByFormat(const QImage& image);
    [[nodiscard]] int getChannelsCountByBytes(const QImage& image);
#endif
} // namespace base::images