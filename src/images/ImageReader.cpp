#include <base/images/ImageReader.h>
#include <base/Utility.h>

#include <base/images/ImagesImage.h>
#include <base/images/ImagesUtility.h>

#include <base/images/formats/BmpHandler.h>
#include <base/images/formats/JpegHandler.h>

#include <base/images/formats/PngHandler.h>

#include <turbojpeg.h>
#include <png.h>

#define JPEG_FORMAT_DEPTH 24
#define JPEG_MONOCHROME_DEPTH 8

#define JPEG_BPP 8
#define STBI_AUTO_DETECT_CHANNELS_COUNT 0


namespace base::images {
	class ImageReaderPrivate {
	public:
	#ifndef LIB_BASE_IMAGES_NO_GIF
			// 
	#endif

	#ifndef LIB_BASE_IMAGES_NO_PNG
		
	#endif

	#ifndef LIB_BASE_IMAGES_NO_JPEG
	
	#endif

	#ifndef LIB_BASE_IMAGES_NO_BMP
	
	#endif


		uchar* loadImage(
			FILE* file,
			ImageData* data);

		void ReadImage(ImageData* data);

		const char* _path = nullptr;
	};


	void ImageReaderPrivate::ReadImage(ImageData* data)
	{
		if (data == nullptr || data->path.has_value() == false)
			return;

		
	}
	
	ImageReader::ImageReader(const char* path):
		_private(new ImageReaderPrivate())
	{
		_private->_path = path;
	}

	void ImageReader::read(Image* image) {
		_private->ReadImage(image->data_ptr());
	}
} // namespace base::images

