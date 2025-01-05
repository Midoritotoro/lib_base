#pragma once 

#include <base/qt/common/Types.h>
#include <QImage>


namespace base::qt::images {
	inline constexpr auto kImageFormat = QImage::Format_ARGB32_Premultiplied;
	inline constexpr auto kMaxScaleByAspectRatio = 16;

	inline constexpr auto kPixelBytesSize = 4;
	inline constexpr auto kAlignImageBy = 64;

	void AlignedImageBufferCleanupHandler(void* data);
	[[nodiscard]] bool IsAlignedImage(const QImage& image);

	void UnPremultiplyLine(uchar* dst, const uchar* src, int intsCount);
	void PremultiplyLine(uchar* dst, const uchar* src, int intsCount);

	void UnPremultiply(QImage& dst, const QImage& src);
	void PremultiplyInplace(QImage& image);

	[[nodiscard]] bool GoodStorageForFrame(const QImage& storage, QSize size);
	[[nodiscard]] QImage CreateFrameStorage(QSize size);
} // namespace base::qt::images