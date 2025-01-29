#pragma once 

#include <base/images/Images.h>
#include <string>


namespace base::images {
	inline constexpr auto kForceImageChannels = 0; // RGB

	class IntegralImage;
	class Image {
	public:
		enum class ColorSpace : uchar {
			Mono = 0x01,
			RGB = 0x02,
			RGBA = 0x04,
			YCbCr = 0x08,
			YUV = 0x10,
			CMY = 0x20,
			CMYK = 0x40,
			HSL = 0x80,
			HSV = 0xFF
		};

		struct ImageData {
			int32 width = 0;
			int32 height = 0;

			uchar* data = nullptr;
			ushort channels = kForceImageChannels;

			int32 depth = 1;
			sizetype bytesPerLine = 0;

			sizetype sizeInBytes = 0;
			int32 devicePixelRatio = 1;

			ColorSpace colorSpace;
			std::vector<Rgb> colorTable;

			std::optional<std::string> path;

			// png, jpeg, bmp, gif
			const char* imageExtension = nullptr;
		};

		Image();
		Image(const std::string& path);

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
		void loadFromFile(const std::string& path);

		void resize(int32 width, int32 height);
		void resize(Size<int32> size);

		//!
		//! \brief
		//! \param jpgQuality игнорируется, если изображение в отличном от jpg формате
		void save(const std::string& path, ushort jpgQuality = 50);
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

		[[nodiscard]] int32
			recountBytesPerLine(
				int32 width, int32 height, int32 depth);

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

		ImageData* _data = nullptr;
	};
} // namespace base::images