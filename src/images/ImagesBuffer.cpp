#include <base/images/ImagesBuffer.h>

#if defined(LIB_BASE_ENABLE_QT)

__BASE_IMAGES_NAMESPACE_BEGIN

void AlignedImageBufferCleanupHandler(void* data) {
	const auto buffer = static_cast<uchar*>(data);
	delete[] buffer;
}

[[nodiscard]] bool IsAlignedImage(const QImage& image) {
	return !(reinterpret_cast<uintptr_t>(image.bits()) % kAlignImageBy)
		&& !(image.bytesPerLine() % kAlignImageBy);
}

void UnPremultiplyLine(
	uchar* dst,
	const uchar* src,
	int intsCount) 
{
	[[maybe_unused]] const auto udst = reinterpret_cast<uint*>(dst);
	const auto usrc = reinterpret_cast<const uint*>(src);

	for (auto i = 0; i != intsCount; ++i)
		udst[i] = qUnpremultiply(usrc[i]);
}

void PremultiplyLine(
	uchar* dst,
	const uchar* src,
	int intsCount)
{
	const auto udst = reinterpret_cast<uint*>(dst);
	[[maybe_unused]] const auto usrc = reinterpret_cast<const uint*>(src);

	for (auto i = 0; i != intsCount; ++i)
		udst[i] = qPremultiply(usrc[i]);
}

void UnPremultiply(
	QImage& dst,
	const QImage& src)
{
	if (!GoodStorageForFrame(dst, src.size()))
		dst = CreateFrameStorage(src.size());

	const auto srcPerLine = src.bytesPerLine();
	const auto dstPerLine = dst.bytesPerLine();

	const auto width = src.width();
	const auto height = src.height();

	auto srcBytes = src.bits();
	auto dstBytes = dst.bits();

	if (srcPerLine != width * 4 || dstPerLine != width * 4)
		for (auto i = 0; i != height; ++i) {
			UnPremultiplyLine(dstBytes, srcBytes, width);

			srcBytes += srcPerLine;
			dstBytes += dstPerLine;
		}
	else
		UnPremultiplyLine(dstBytes, srcBytes, width * height);
}

void PremultiplyInplace(QImage& image) {
	const auto perLine = image.bytesPerLine();
	auto bytes = image.bits();

	const auto width = image.width();
	const auto height = image.height();

	if (perLine != width * 4)
		for (auto i = 0; i != height; ++i) {
			PremultiplyLine(bytes, bytes, width);
			bytes += perLine;
		}
	else
		PremultiplyLine(bytes, bytes, width * height);
}

bool GoodStorageForFrame(
	const QImage& storage,
	QSize size)
{
	return !storage.isNull()
		&& (storage.format() == kImageFormat)
		&& (storage.size() == size)
		&& storage.isDetached()
		&& IsAlignedImage(storage);
}

QImage CreateFrameStorage(QSize size) {
	const auto width = size.width();
	const auto height = size.height();

	const auto widthAlign = kAlignImageBy / kPixelBytesSize;
	const auto neededWidth = width + ((width % widthAlign)
		? (widthAlign - (width % widthAlign))
		: 0);

	const auto perLine = neededWidth * kPixelBytesSize;

	const auto buffer = new uchar[perLine * height + kAlignImageBy];
	const auto cleanupData = static_cast<void*>(buffer);

	const auto address = reinterpret_cast<uintptr_t>(buffer);
	const auto alignedBuffer = buffer + ((address % kAlignImageBy)
		? (kAlignImageBy - (address % kAlignImageBy))
		: 0);

	return QImage(
		alignedBuffer, width, height,
		perLine, kImageFormat,
		AlignedImageBufferCleanupHandler,
		cleanupData);
}

__BASE_IMAGES_NAMESPACE_END

#endif