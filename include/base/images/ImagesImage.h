#pragma once 

#include <base/images/Images.h>


namespace base::images {
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

		void resize(int32 width, int32 height);
		void resize(Size<int32> size);

		static [[nodiscard]] std::vector<const char*> supportedFormats() noexcept;

		//!
		//! \brief
		//! \param quality - качество декомпрессии jpeg изображения (от 5 до 100)
		void setJpegQuality(ushort quality);
		[[nodiscard]] ushort jpegQuality() const noexcept;

		//!
		//! \brief
		//! \param format - jpg(jpeg), png, bmp
		void convertToFormat(const char* format);
		[[nodiscard]] const char* format() const noexcept;

		void save(const char* path);
		[[nodiscard]] Image convertToColorSpace(ColorSpace space) const;

		[[nodiscard]] Rect<int32> rect() const noexcept;
		[[nodiscard]] Size<int32> size() const noexcept;

		[[nodiscard]] int32 width() const noexcept;
		[[nodiscard]] int32 height() const noexcept;

		[[nodiscard]] int32 bytesPerLine() const noexcept;

		[[nodiscard]] uchar* bytesData();
		[[nodiscard]] ImageData* data_ptr();

		[[nodiscard]] uchar* scanLine(int i);
		[[nodiscard]] const uchar* scanLine(int i) const;

		[[nodiscard]] bool isNull() const noexcept;
		[[nodiscard]] Rgb pixel(int x, int y) const;

	private:
		bool isEqual(const Image& other) const;

		ImageData* _data = nullptr;
	};
} // namespace base::images