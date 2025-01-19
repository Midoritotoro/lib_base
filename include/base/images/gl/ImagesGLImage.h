#pragma once 


#include <base/images/Images.h>
#include <string>

namespace base::images {
	inline constexpr auto kForceImageChannels = 4; // rgba

	class IntegralImage;

	class GLImage final {
	public:
		enum class Format : uchar {
			Format_Mono,
			Format_RGB32,
			Format_ARGB32,
			Format_ARGB32_Premultiplied
		};

		struct GLImageSizeParameters{
			sizetype bytesPerLine;

			bool isValid() const { 
				return bytesPerLine > 0; 
			}
		};

		struct GLImageData {
			int32 width = 0;
			int32 height = 0;

			::uchar* data = nullptr;
			// Format format;

			ushort channels = 0;

			int32 depth = 0;
			sizetype bytesPerLine = 0;

			sizetype totalSize = 0;
			int32 devicePixelRatio = 0;

			Format format;
			std::vector<Rgb> colorTable;
		};

		GLImage();

		GLImage(GLImage&& image);
		GLImage(const IntegralImage& image);

		GLImage(::uchar* data);

		GLImage(const Size<int32>& size);
		GLImage(int32 width, int32 height);

		GLImage(
			::uchar * data,
			int32 width, int32 height);
		GLImage(
			::uchar* data,
			const Size<int32>& size);

#ifdef LIB_BASE_ENABLE_QT
		GLImage(QImage&& image);
#endif
		GLImage& operator=(const GLImage& other) = default;

		bool operator==(const GLImage& other);
		bool operator!=(const GLImage& other);

		void loadFromData(::uchar* data);
		void loadFromFile(const std::string& path);

		[[nodiscard]] GLImage convertToFormat(Format format) const;

		[[nodiscard]] Rect<int32> rect() const noexcept;
		[[nodiscard]] Size<int32> size() const noexcept;

		[[nodiscard]] int32 width() const noexcept;
		[[nodiscard]] int32 height() const noexcept;

		[[nodiscard]] int32 bytesPerLine() const noexcept;

		[[nodiscard]] ::uchar* bytesData();
		[[nodiscard]] GLImageData* data();

		[[nodiscard]] uchar* scanLine(int i);
		[[nodiscard]] const uchar* scanLine(int i) const;

		[[nodiscard]] Rgb pixel(int x, int y) const;
	private:
		bool isEqual(const GLImage& other) const;

		[[nodiscard]] GLImageSizeParameters 
			recountImageParameters(
				int32 width, int32 height, int32 depth);

		GLImageData* _data = nullptr;
	};
} // namespace base::images