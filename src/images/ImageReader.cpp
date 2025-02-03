#include <base/images/ImageReader.h>
#include <base/Utility.h>

#include <base/images/ImagesImage.h>
#include <base/images/ImagesUtility.h>

#include <base/images/formats/BmpHandler.h>
#include <base/images/formats/JpegHandler.h>

#include <base/images/formats/PngHandler.h>

#include <turbojpeg.h>
#include <png.h>

#include <cstdio>

#include <qDebug>


// Все возвращаемые этим файлом исключения можно отключить,
// определив LIB_BASE_IMAGES_NO_FAILURE, в противном случае ошибки, возникшие здесь вызовут std::abortxs


#ifdef LIB_BASE_IMAGES_NO_FAILURE
#define IMAGES_READER_ASSERT(cond)					0
#define IMAGES_READER_ASSERT_LOG(cond, mes)
#else
#define IMAGES_READER_ASSERT						Assert
#define IMAGES_READER_ASSERT_LOG					AssertLog
#endif


#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
extern "C" __declspec(dllimport) int __stdcall MultiByteToWideChar(unsigned int cp, unsigned long flags, const char* str, int cbmb, wchar_t* widestr, int cchwide);
extern "C" __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int cp, unsigned long flags, const wchar_t* widestr, int cchwide, char* str, int cbmb, const char* defchar, int* used_default);
#endif

#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
int ConvertWCharToUnicode(char* buffer, size_t bufferlen, const wchar_t* input)
{
	return WideCharToMultiByte(65001 /* UTF8 */, 0, input, -1, buffer, (int)bufferlen, NULL, NULL);
}

int ConvertUnicodeToWChar(wchar_t* buffer, const char* input)
{
	return MultiByteToWideChar(65001 /* UTF8 */, 0, input, -1, buffer, sizeof(buffer) / sizeof(*buffer));
}
#endif


static FILE* readerFileOpen(char const* filename, char const* mode)
{
	FILE* f = nullptr;
#if defined(_WIN32) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
	wchar_t wMode[64];
	wchar_t wFilename[1024];
	if (ConvertUnicodeToWChar(wFilename, filename) == 0)
		return nullptr;

	if (ConvertUnicodeToWChar(wMode, mode) == 0)
		return nullptr;

#if defined(_MSC_VER) && _MSC_VER >= 1400
	if (0 != _wfopen_s(&f, wFilename, wMode))
		f = 0;
#else
	f = _wfopen(wFilename, wMode);
#endif

#elif defined(_MSC_VER) && _MSC_VER >= 1400
	if (0 != fopen_s(&f, filename, mode))
		f = 0;
#else
	f = fopen(filename, mode);
#endif
	return f;
}

namespace base::images {
	class ImageReaderPrivate {
	public:
		//! \return Handler, соответствуюший определенному формату изображения.
		//! В случае, если не удается определелить формат, изображение повреждено или формат не поддерживается, nullptr
		[[nodiscard]] AbstractFormatHandler* createFormatHandler(ImageData* data);

	#ifndef LIB_BASE_IMAGES_NO_GIF
		bool checkGifHeader(uchar* header);
			// 
	#endif

	#ifndef LIB_BASE_IMAGES_NO_PNG
		bool checkPngHeader(uchar* header);
	#endif

	#ifndef LIB_BASE_IMAGES_NO_JPEG
		bool checkJpegHeader(uchar* header);
	#endif

	#ifndef LIB_BASE_IMAGES_NO_BMP
		bool checkBmpHeader(uchar* header);
	#endif


		void ReadImage(ImageData* data);

		const char* _path = nullptr;
	};

	AbstractFormatHandler* ImageReaderPrivate::createFormatHandler(ImageData* data) {
		FILE* file = readerFileOpen(data->path.value(), "rb");
		if (!file)
			return nullptr;

		uchar* header;
		int readed = fread(header, 1, 8, file);
		bool is_png = !png_sig_cmp(header, 0, 8);
		
		printf("ImageReaderPrivate::createFormatHandler readed bytes: %i\n", readed);
		fseek(file, SEEK_SET, SEEK_CUR);

		
		qDebug() << "is_png: " << is_png;
	}


	void ImageReaderPrivate::ReadImage(ImageData* data)
	{
		if (data == nullptr || data->path.has_value() == false)
			return;
		
		createFormatHandler(data);
		data->handler->read(data, data->path.value());
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
