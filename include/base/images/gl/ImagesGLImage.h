#pragma once 


#include <base/images/Images.h>

#include <string>


namespace base::images {
	inline constexpr auto kForceImageChannels = 4; // rgba

	class IntegralImage;

	class GLImage final {
	public:
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
		};

		GLImage();

		GLImage(const GLImage& image);
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
		GLImage(const QImage& image);
#endif

		GLImage& operator=(const GLImage& other) = default;

		bool operator==(const GLImage& other);
		bool operator!=(const GLImage& other);

		void loadFromData(::uchar* data);
		void loadFromFile(const std::string& path);

		[[nodiscard]] Size<int32> size() const noexcept;

		[[nodiscard]] int32 width() const noexcept;
		[[nodiscard]] int32 height() const noexcept;

		[[nodiscard]] int32 bytesPerLine() const noexcept;

		[[nodiscard]] ::uchar* bytesData();
		[[nodiscard]] GLImageData* data();
	private:
		bool isEqual(const GLImage& other) const;

		[[nodiscard]] GLImageSizeParameters 
			recountImageParameters(
				int32 width, int32 height, int32 depth);

		GLImageData* _data = nullptr;
	};
} // namespace base::images