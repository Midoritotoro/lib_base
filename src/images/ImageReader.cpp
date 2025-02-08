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
	struct ImageContext
	{
		uchar buffer_start[IMAGE_HEADER_SIZE_IN_BYTES] = {};
		uchar* img_buffer = nullptr, * img_buffer_end = nullptr;
		uchar* img_buffer_original = nullptr;
	};

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
	static int get8(ImageContext* s) {
		if (s->img_buffer < s->img_buffer_end)
			return *s->img_buffer++;
		return 0;
	}

	static int get16le(ImageContext* s)
	{
		int z = get8(s);
		return z + (get8(s) << 8);
	}

	static uint32 get32le(ImageContext* s)
	{
		uint32 z = get16le(s);
		z += (uint32)get16le(s) << 16;
		return z;
	}

	static int bmp_test_raw(ImageContext* s)
	{
		int r;
		int sz;

		if (get8(s) != 'B') {
			printf("get8(s) != B\n");
			return 0;
		}

		if (get8(s) != 'M') {
			printf("get8(s) != M\n");
			return 0;
		}

		int size = get32le(s); // discard filesize
		std::cout << "bmp size: " << size;
		get16le(s); // discard reserved
		get16le(s); // discard reserved
		get32le(s); // discard data offset

		sz = get32le(s);
		r = (sz == 12 || sz == 40 || sz == 56 || sz == 108 || sz == 124);
		return r;
	}

	bool ImageReaderPrivate::checkBmpHeader(_SAL2_In_reads_bytes_(IMAGE_HEADER_SIZE_IN_BYTES) uchar* header) {
		int r;
		int sz;

		if (get8(s) != 'B') {
			printf("get8(s) != B\n");
			return 0;
		}

		if (get8(s) != 'M') {
			printf("get8(s) != M\n");
			return 0;
		}

		int size = get32le(s); // discard filesize
		std::cout << "bmp size: " << size;
		get16le(s); // discard reserved
		get16le(s); // discard reserved
		get32le(s); // discard data offset

		sz = get32le(s);
		r = (sz == 12 || sz == 40 || sz == 56 || sz == 108 || sz == 124);

		ctx->img_buffer = ctx->img_buffer_original;

		return r;
	}
#endif

	// NOTE: In bmp images all of the integer values are stored in little-endian format (i.e. least-significant byte first).
	static int bmp_get8(BmpContext* s) {
		if (s->img_buffer < s->img_buffer_end)
			return *s->img_buffer++;
		return 0;
	}

	static int bmp_get16le(BmpContext* s)
	{
		int z = get8(s);
		return z + (get8(s) << 8);
	}

	static uint32 bmp_get32le(BmpContext* s)
	{
		uint32 z = get16le(s);
		z += (uint32)get16le(s) << 16;
		return z;
	}



	AbstractFormatHandler* ImageReaderPrivate::createFormatHandler(ImageData* data) {
		ImageContext context;
		
		io::File file = io::File(data->path.value());
		const auto isOpened = file.open(io::FileOpenMode::Read | io::FileOpenMode::Binary);

		ImagesAssert(isOpened, "base::images::ImageReaderPrivate::createFormatHandler: Не удается открыть файл изображения. ", nullptr);

		//uchar* header = new uchar[IMAGE_HEADER_SIZE_IN_BYTES];
		sizetype res = file.read(context.buffer_start, sizeof(context.buffer_start));
		if (res != 0) {
			context.img_buffer = context.buffer_start;
			context.img_buffer_end = context.img_buffer + res;
		}


		bmp_test(&context);


	/*	if (checkPngHeader(header))
			return new PngHandler();

		else if (checkBmpHeader(header))
			return new BmpHandler();

		else if (checkJpegHeader(header))
			return new JpegHandler();*/

		ImagesAssert(false, "base::images::ImageReaderPrivate::createFormatHandler: Файл не является изображением, или же его расширение не поддерживается. ", nullptr);
		return new PngHandler;
	}

	void ImageReaderPrivate::ReadImage(ImageData* data)
	{
		if (data == nullptr || data->path.has_value() == false)
			return;
		
		AbstractFormatHandler* formatHandler = createFormatHandler(data);

	/*	data->handler = formatHandler;
		data->handler->read(data);*/
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
