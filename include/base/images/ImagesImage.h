#pragma once 

#include <base/images/Images.h>
#include <string>


namespace base::images {
	inline constexpr auto kForceImageChannels = 0; // default
	inline constexpr auto kDefaultStbiJpegQuality = 30;

	class IntegralImage;
	class Image {
	public:
		enum class ColorSpace : uchar {
			Invalid = 0x00,
			BGR = 0x01,
			Mono = 0x02,
			RGB = 0x04,
			RGBA = 0x08,
			/*YCbCr = 0x08,
			YUV = 0x10,
			CMY = 0x20,
			CMYK = 0x40,
			HSL = 0x80,
			HSV = 0xFF*/
		};

		struct ImageData {
			int32 width = 0;
			int32 height = 0;

			uchar* data = nullptr;
			ushort channels = kForceImageChannels;

			int32 depth = 1;
			int32 bytesPerLine = 0;

			int32 sizeInBytes = 0;
			sizetype dataLength = 0;

			int32 devicePixelRatio = 1;

			int32 bitsPerChannel = 0;

			ColorSpace colorSpace;
			std::vector<Rgb> colorTable;

			std::optional<const char*> path;

			// png, jpeg, bmp
			const char* imageExtension = nullptr;
		};

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

		static [[nodiscard]] bool isJpeg(const char* ext);
		static [[nodiscard]] bool isPng(const char* ext);
		static [[nodiscard]] bool isBmp(const char* ext);

		static [[nodiscard]] bool isFormatSupported(const char* format);
		static [[nodiscard]] bool isFormatsEqual(
			const char* firstFormat, 
			const char* secondFormat);

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

		[[nodiscard]] int32 recountBytesPerLine(ImageData* data);

		void readImage(
			ImageData* data,
			int32 forceChannelsCount = kForceImageChannels);

		[[nodiscard]] uchar* loadImage(
			FILE* file,
			ImageData* data, 
			int32 forceChannelsCount = kForceImageChannels);

		void writeImageToFile(
			ImageData* data,
			const std::string& path);

		[[nodiscard]] std::string getExtensionFromPath(const std::string& path);

		ImageData* _data = nullptr;
		ushort _jpegQuality = kDefaultStbiJpegQuality;
	};
} // namespace base::images