#pragma once 

#include <base/images/Images.h>


__BASE_IMAGES_NAMESPACE_BEGIN

class IntegralImage;
class Image {
public:
	Image();
	Image(const char* path);

	Image(Image&& image) noexcept;
	Image(const IntegralImage& image);

	Image(uchar* data);

	Image(const Size<int32>& size);
	Image(int32 width, int32 height);

	Image(
		uchar* data,
		int32 width, int32 height);
	Image(
		uchar* data,
		const Size<int32>& size);

	Image(ImageData* data);

#ifdef LIB_BASE_ENABLE_QT
	Image(QImage&& image);
#endif

	~Image();

	Image& operator=(const Image& other) = default;

	bool operator==(const Image& other);
	bool operator!=(const Image& other);

	void loadFromData(const uchar* data, sizetype length);

	void loadFromFile(const char* path);

	// Alias to loadFromFile
	void load(const char* path);

	void resize(int32 width, int32 height);
	void resize(Size<int32> size);

	static NODISCARD std::vector<const char*> supportedFormats() noexcept;

	//!
	//! \brief
	//! \param quality - качество декомпрессии jpeg изображения (от 5 до 100)
	void setJpegQuality(ushort quality);
	NODISCARD ushort jpegQuality() const noexcept;

	//!
	//! \brief
	//! \param format - jpg(jpeg), png, bmp
	void convertToFormat(const char* format);
	NODISCARD const char* format() const noexcept;

	void setFilter(Filter filter);
	NODISCARD Filter filter() const noexcept;

	NODISCARD bool hasFilter() const noexcept;

	void save(const char* path);
	NODISCARD Image convertToColorSpace(ColorSpace space) const;

	NODISCARD Rect<int32> rect() const noexcept;
	NODISCARD Size<int32> size() const noexcept;

	NODISCARD int32 width() const noexcept;
	NODISCARD int32 height() const noexcept;

	NODISCARD int32 bytesPerLine() const noexcept;

	NODISCARD uchar* bytesData();
	NODISCARD ImageData* data_ptr();

	NODISCARD uchar* scanLine(int i);
	NODISCARD const uchar* scanLine(int i) const;

	NODISCARD bool isNull() const noexcept;
	NODISCARD Rgb pixel(int x, int y) const;

	NODISCARD int32 depth() const noexcept;
	NODISCARD ushort channels() const noexcept;

	NODISCARD int32 sizeInBytes() const noexcept;
	NODISCARD sizetype dataLength() const noexcept;

	NODISCARD int32 bitsPerChannel() const noexcept;
	NODISCARD ColorSpace colorSpace() const noexcept;

	NODISCARD bool hasPath() const noexcept;
	NODISCARD const char* path() const noexcept;
private:
	bool isEqual(const Image& other) const;

	ImageData* _data = nullptr;
};

__BASE_IMAGES_NAMESPACE_END
