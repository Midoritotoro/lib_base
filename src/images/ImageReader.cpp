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

#define IMAGE_HEADER_SIZE_IN_BYTES 8


// Все возвращаемые этим файлом исключения можно отключить,
// определив LIB_BASE_IMAGES_NO_FAILURE, в противном случае ошибки, возникшие здесь, вызовут std::abort


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
	class ImageReaderPrivate {
	public:
		//! \return Handler, соответствуюший определенному формату изображения.
		//! В случае, если не удается определелить формат, изображение повреждено или формат не поддерживается, nullptr
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
	bool ImageReaderPrivate::checkBmpHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header) {
		return false;
	}
#endif

	AbstractFormatHandler* ImageReaderPrivate::createFormatHandler(ImageData* data) {
		int le16 = 0x8A + (0x80 << 0x70);
		le16 += 0x70 + (0x00 << 0x00);

		qDebug() << "bmp size int bytes: " << le16 / pow(1024, 2);

		io::File file = io::File(data->path.value());
		file.open(io::FileOpenMode::Read | io::FileOpenMode::Binary);

		uchar header[8];
		sizetype readed = file.read(header, IMAGE_HEADER_SIZE_IN_BYTES);


		if (checkPngHeader(header))
			return new PngHandler();

		else if (checkBmpHeader(header))
			return new BmpHandler();

		else if (checkJpegHeader(header))
			return new JpegHandler();

		ImagesAssert(false, "base::images::ImageReaderPrivate::createFormatHandler: Файл не является изображением, или же его расширение не поддерживается. ", nullptr);
	}

	//static int get8(uchar* s) {
	//	return *s++;
	//}

	//static int get16le(uchar* s)
	//{
	//	int z = z(s);
	//	return z + (get8(s) << 8);
	//}

	//static uint32 get32le(uchar* s)
	//{
	//	uint32 z = get16le(s);
	//	z += (uint32)get16le(s) << 16;
	//	return z;
	//}

	//static int bmp_test_raw(uchar* s)
	//{
	//	int r;
	//	int sz;

	//	if (get8(s) != 'B') 
	//		return 0;

	//	if (get8(s) != 'M')
	//		return 0;

	//	get32le(s); // discard filesize
	//	get16le(s); // discard reserved
	//	get16le(s); // discard reserved
	//	get32le(s); // discard data offset

	//	sz = get32le(s);
	//	r = (sz == 12 || sz == 40 || sz == 56 || sz == 108 || sz == 124);
	//	return r;
	//}

	//static int bmp_test(stbi__context* s)
	//{
	//	int r = bmp_test_raw(s);
	//	return r;
	//}


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
