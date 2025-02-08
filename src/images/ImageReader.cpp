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

// Пока отложил этот файл на потом


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
		// В bmp формате всегда используется little-endian порядок байт https://ru.wikipedia.org/wiki/BMP
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

		// В начале каждого bmp файла находится сигнатура формата размером в два байта.
		// Она может содержать единственное значение 0x4D42 (little-endian)

		if (bmpGet8(s) != 'B')
			return false;

		if (bmpGet8(s) != 'M')
			return false;
		

		bmpGet32LittleEndian(s); // Пропускаем размер файла
		bmpGet16LittleEndian(s); // Пропускаем зарезервированное пространство

		bmpGet16LittleEndian(s); // Пропускаем зарезервированное пространство
		bmpGet32LittleEndian(s); // Пропускаем смещение данных

		sz = bmpGet32LittleEndian(s); // Размер заголовка bmp файла

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

		ImagesAssert(isOpened, "base::images::ImageReaderPrivate::createFormatHandler: Не удается открыть файл изображения. ", nullptr);

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

		ImagesAssert(false, "base::images::ImageReaderPrivate::createFormatHandler: Файл не является изображением, или же его расширение не поддерживается. ", nullptr);
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
