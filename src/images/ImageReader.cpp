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

#define IMAGE_HEADER_SIZE_IN_BYTES 8


// ��� ������������ ���� ������ ���������� ����� ���������,
// ��������� LIB_BASE_IMAGES_NO_FAILURE, � ��������� ������ ������, ��������� �����, ������� std::abort


#ifdef LIB_BASE_IMAGES_NO_FAILURE

#define IMAGES_READER_ASSERT(cond)					0
#define IMAGES_READER_ASSERT_LOG(cond, mes)
#else
#define IMAGES_READER_ASSERT						Assert
#define IMAGES_READER_ASSERT_LOG					AssertLog

#endif


#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)

extern "C" __declspec(dllimport) int __stdcall MultiByteToWideChar(
	unsigned int cp, unsigned long flags,
	const char* str, int cbmb, wchar_t* widestr, int cchwide);

extern "C" __declspec(dllimport) int __stdcall WideCharToMultiByte(
	unsigned int cp, unsigned long flags, const wchar_t* widestr, 
	int cchwide, char* str, int cbmb, const char* defchar, int* used_default);

#endif

#if defined(OS_WIN) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)

	int ConvertWCharToUnicode(
		char* buffer, 
		size_t bufferlen,
		const wchar_t* input)
	{
		return WideCharToMultiByte(
			65001 /* UTF8 */, 0, input, -1,
			buffer, (int)bufferlen, NULL, NULL);
	}

	int ConvertUnicodeToWChar(
		wchar_t* buffer,
		const char* input)
	{
		return MultiByteToWideChar(
			65001 /* UTF8 */, 0, input, -1,
			buffer, sizeof(buffer) / sizeof(*buffer));
	}

#endif


static FILE* readerFileOpen(
	char const* filename,
	char const* mode)
{
	FILE* f = nullptr;
/*#if defined(_WIN32) && defined(LIB_BASE_ENABLE_WINDOWS_UNICODE)
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

#el*/#if defined(_MSC_VER) && _MSC_VER >= 1400
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
		//! \return Handler, ��������������� ������������� ������� �����������.
		//! � ������, ���� �� ������� ������������ ������, ����������� ���������� ��� ������ �� ��������������, nullptr
		[[nodiscard]] AbstractFormatHandler* createFormatHandler(ImageData* data);

	#ifndef LIB_BASE_IMAGES_NO_GIF
		[[nodiscard]] bool checkGifHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header);
			// 
	#endif

	#ifndef LIB_BASE_IMAGES_NO_PNG
		[[nodiscard]] bool checkPngHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header);
	#endif

	#ifndef LIB_BASE_IMAGES_NO_JPEG
		[[nodiscard]] bool checkJpegHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header);
	#endif

	#ifndef LIB_BASE_IMAGES_NO_BMP
		[[nodiscard]] bool checkBmpHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header);
	#endif

	#ifndef LIB_BASE_IMAGES_NO_ICO

	#endif


		void ReadImage(ImageData* data);

		const char* _path = nullptr;
	};

#ifndef LIB_BASE_IMAGES_NO_GIF
	bool ImageReaderPrivate::checkGifHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header) {
		return true;
	}
#endif

#ifndef LIB_BASE_IMAGES_NO_PNG
	bool ImageReaderPrivate::checkPngHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header) {
		return !png_sig_cmp(header, 0, IMAGE_HEADER_SIZE_IN_BYTES);
	}
#endif

#ifndef LIB_BASE_IMAGES_NO_JPEG
	bool ImageReaderPrivate::checkJpegHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header) {

	}
#endif

#ifndef LIB_BASE_IMAGES_NO_BMP
	bool ImageReaderPrivate::checkBmpHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header) {
		
	}
#endif


	AbstractFormatHandler* ImageReaderPrivate::createFormatHandler(ImageData* data) {
		FILE* file = readerFileOpen(data->path.value(), "rb");
		IMAGES_READER_ASSERT_LOG(file != nullptr, "base::images::ImageReaderPrivate::createFormatHandler: Cannot fopen. Check if the path is correct. ");

		uchar header[8];
		int readed = fread(header, 1, 8, file);

		printf("ImageReaderPrivate::createFormatHandler readed bytes: %i\n", readed);
		fseek(file, SEEK_SET, SEEK_CUR);

		qDebug() << "is_png: " << checkPngHeader(header);

		return nullptr;
	}


	void ImageReaderPrivate::ReadImage(ImageData* data)
	{
		if (data == nullptr || data->path.has_value() == false)
			return;
		
		createFormatHandler(data);
		// data->handler->read(data, data->path.value());
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
