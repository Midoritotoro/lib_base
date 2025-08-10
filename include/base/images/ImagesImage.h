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

	static   std::vector<const char*> supportedFormats() noexcept;

	//!
	//! \brief
	//! \param quality - качество декомпрессии jpeg изображения (от 5 до 100)
	void setJpegQuality(ushort quality);
	  ushort jpegQuality() const noexcept;

	//!
	//! \brief
	//! \param format - jpg(jpeg), png, bmp
	void convertToFormat(const char* format);
	  const char* format() const noexcept;

	void setFilter(Filter filter);
	  Filter filter() const noexcept;

	  bool hasFilter() const noexcept;

	void save(const char* path);
	  Image convertToColorSpace(ColorSpace space) const;

	  Rect<int32> rect() const noexcept;
	  Size<int32> size() const noexcept;

	  int32 width() const noexcept;
	  int32 height() const noexcept;

	  int32 bytesPerLine() const noexcept;

	  uchar* bytesData();
	  ImageData* data_ptr();

	  uchar* scanLine(int i);
	  const uchar* scanLine(int i) const;

	  bool isNull() const noexcept;
	  Rgb pixel(int x, int y) const;

	  int32 depth() const noexcept;
	  ushort channels() const noexcept;

	  int32 sizeInBytes() const noexcept;
	  sizetype dataLength() const noexcept;

	  int32 bitsPerChannel() const noexcept;
	  ColorSpace colorSpace() const noexcept;

	  bool hasPath() const noexcept;
	  const char* path() const noexcept;
private:
	bool isEqual(const Image& other) const;

	ImageData* _data = nullptr;
};

__BASE_IMAGES_NAMESPACE_END
