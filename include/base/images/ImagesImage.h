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

	static base_nodiscard std::vector<const char*> supportedFormats() noexcept;

	//!
	//! \brief
	//! \param quality - качество декомпрессии jpeg изображения (от 5 до 100)
	void setJpegQuality(ushort quality);
	base_nodiscard ushort jpegQuality() const noexcept;

	//!
	//! \brief
	//! \param format - jpg(jpeg), png, bmp
	void convertToFormat(const char* format);
	base_nodiscard const char* format() const noexcept;

	void setFilter(Filter filter);
	base_nodiscard Filter filter() const noexcept;

	base_nodiscard bool hasFilter() const noexcept;

	void save(const char* path);
	base_nodiscard Image convertToColorSpace(ColorSpace space) const;

	base_nodiscard Rect<int32> rect() const noexcept;
	base_nodiscard Size<int32> size() const noexcept;

	base_nodiscard int32 width() const noexcept;
	base_nodiscard int32 height() const noexcept;

	base_nodiscard int32 bytesPerLine() const noexcept;

	base_nodiscard uchar* bytesData();
	base_nodiscard ImageData* data_ptr();

	base_nodiscard uchar* scanLine(int i);
	base_nodiscard const uchar* scanLine(int i) const;

	base_nodiscard bool isNull() const noexcept;
	base_nodiscard Rgb pixel(int x, int y) const;

	base_nodiscard int32 depth() const noexcept;
	base_nodiscard ushort channels() const noexcept;

	base_nodiscard int32 sizeInBytes() const noexcept;
	base_nodiscard sizetype dataLength() const noexcept;

	base_nodiscard int32 bitsPerChannel() const noexcept;
	base_nodiscard ColorSpace colorSpace() const noexcept;

	base_nodiscard bool hasPath() const noexcept;
	base_nodiscard const char* path() const noexcept;
private:
	bool isEqual(const Image& other) const;

	ImageData* _data = nullptr;
};

__BASE_IMAGES_NAMESPACE_END
