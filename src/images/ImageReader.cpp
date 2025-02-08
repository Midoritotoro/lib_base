#include <base/images/ImageReader.h>
#include <base/Utility.h>

#include <base/images/ImagesImage.h>
#include <base/images/ImagesUtility.h>

#include <base/images/formats/BmpHandler.h>
#include <base/images/formats/JpegHandler.h>

#include <base/images/formats/PngHandler.h>

#include <base/io/File.h>

#include <turbojpeg.h>
#include <png.h>

#include <cstdio>

#include <qDebug>

#define IMAGE_HEADER_SIZE_IN_BYTES 128

// ���� ������� ���� ���� �� �����


// ��� ������������ ���� ������ ���������� ����� ���������,
// ��������� LIB_BASE_IMAGES_NO_FAILURE, � ��������� ������ ������, ��������� �����, ������� std::abort


#ifdef LIB_BASE_IMAGES_NO_FAILURE
	#define ImagesAssert						AssertReturn
#else
	#define ImagesAssert(cond, mes, retval)		(AssertValidationCondition(\
	cond,\
	mes,\
	SOURCE_FILE_BASENAME,\
	__LINE__))
#endif



namespace base::images {
	struct ImageContext
	{
		uchar buffer_start[IMAGE_HEADER_SIZE_IN_BYTES] = {};
		uchar* img_buffer = nullptr, * img_buffer_end = nullptr;
		uchar* img_buffer_original = nullptr;
	};

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
		// � bmp ������� ������ ������������ little-endian ������� ���� https://ru.wikipedia.org/wiki/BMP
		[[nodiscard]] int bmpGet8(ImageContext* s);
		[[nodiscard]] int bmpGet16LittleEndian(ImageContext* s);

		[[nodiscard]] uint32 bmpGet32LittleEndian(ImageContext* s);
		[[nodiscard]] bool bmpTest(ImageContext* s);

		[[nodiscard]] bool checkBmpHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header);
#endif

#ifndef LIB_BASE_IMAGES_NO_ICO

#endif

		void ReadImage(ImageData* data);

		const char* _path = nullptr;
	};

#ifndef LIB_BASE_IMAGES_NO_GIF
	bool ImageReaderPrivate::checkGifHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header) {
		return false;
	}
#endif

#ifndef LIB_BASE_IMAGES_NO_PNG
	bool ImageReaderPrivate::checkPngHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header) {
		return png_check_sig(header, IMAGE_HEADER_SIZE_IN_BYTES);
	}
#endif

#ifndef LIB_BASE_IMAGES_NO_JPEG
	bool ImageReaderPrivate::checkJpegHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header) {
		return false;
	}
#endif

#ifndef LIB_BASE_IMAGES_NO_BMP
	int ImageReaderPrivate::bmpGet8(ImageContext* s) {
		if (s->img_buffer < s->img_buffer_end)
			return *s->img_buffer++;
		return 0;
	}

	int ImageReaderPrivate::bmpGet16LittleEndian(ImageContext* s)
	{
		int z = bmpGet8(s);
		return z + (bmpGet8(s) << 8);
	}

	uint32 ImageReaderPrivate::bmpGet32LittleEndian(ImageContext* s)
	{
		uint32 z = bmpGet16LittleEndian(s);
		z += (uint32)bmpGet16LittleEndian(s) << 16;
		return z;
	}

	bool ImageReaderPrivate::bmpTest(ImageContext* s)
	{
		int sz;

		// � ������ ������� bmp ����� ��������� ��������� ������� �������� � ��� �����.
		// ��� ����� ��������� ������������ �������� 0x4D42 (little-endian)

		if (bmpGet8(s) != 'B')
			return false;

		if (bmpGet8(s) != 'M')
			return false;
		

		bmpGet32LittleEndian(s); // ���������� ������ �����
		bmpGet16LittleEndian(s); // ���������� ����������������� ������������

		bmpGet16LittleEndian(s); // ���������� ����������������� ������������
		bmpGet32LittleEndian(s); // ���������� �������� ������

		sz = bmpGet32LittleEndian(s); // ������ ��������� bmp �����

		return (sz == 12 || sz == 40 || sz == 56 || sz == 108 || sz == 124);
	}

	bool ImageReaderPrivate::checkBmpHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header) {
		ImageContext context;

		context.img_buffer = header;
		context.img_buffer_end = header + IMAGE_HEADER_SIZE_IN_BYTES;

		return bmpTest(&context);
	}
#endif


	AbstractFormatHandler* ImageReaderPrivate::createFormatHandler(ImageData* data) {
	//	ImageContext context;
		
		io::File file = io::File(data->path.value());
		const auto isOpened = file.open(io::FileOpenMode::Read | io::FileOpenMode::Binary);

		ImagesAssert(isOpened, "base::images::ImageReaderPrivate::createFormatHandler: �� ������� ������� ���� �����������. ", nullptr);

		uchar* header = new uchar[IMAGE_HEADER_SIZE_IN_BYTES];
		sizetype res = file.read(header, IMAGE_HEADER_SIZE_IN_BYTES);
		//sizetype res = file.read(context.buffer_start, sizeof(context.buffer_start));
		//if (res != 0) {
		//	context.img_buffer = context.buffer_start;
		//	context.img_buffer_end = context.img_buffer + res;
		//}


		//bmp_test(&context);


		if (checkPngHeader(header))
			return new PngHandler();

		else if (checkBmpHeader(header))
			return new BmpHandler();

		else if (checkJpegHeader(header))
			return new JpegHandler();

		ImagesAssert(false, "base::images::ImageReaderPrivate::createFormatHandler: ���� �� �������� ������������, ��� �� ��� ���������� �� ��������������. ", nullptr);
		return nullptr;
	}

	void ImageReaderPrivate::ReadImage(ImageData* data)
	{
		if (data == nullptr || data->path.has_value() == false)
			return;
		
		AbstractFormatHandler* formatHandler = createFormatHandler(data);

		data->handler = formatHandler;
		data->handler->read(data);
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
