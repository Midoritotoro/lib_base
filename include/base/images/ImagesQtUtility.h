#pragma once 


#ifdef LIB_BASE_ENABLE_QT 
#include <QImage>


namespace base::images {
    int getChannelsCountByFormat(const QImage& image);
    [[nodiscard]] int getChannelsCountByBytes(const QImage& image);
} // namespace base::images
#endif