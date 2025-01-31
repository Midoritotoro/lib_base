#include <base/images/ImagesImage.h>

#include <base/images/ImagesUtility.h>

#include <base/Utility.h>
#include <qDebug>

#define STB_IMAGE_IMPLEMENTATION
#include <base/images/stb/StbImage.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <base/images/stb/StbImageWrite.h>

#define STB_IMAGE_RESIZE2_IMPLEMENTATION
#include <base/images/stb/StbImageResize.h>


#include <regex>



namespace base::images {
	namespace {
		static [[nodiscard]] void* loadPng(
			stbi__context* s, 
			Image::ImageData* data, 
			ushort forceChannelsCount,
			stbi__result_info* ri)
		{
			stbi__png p;
			p.s = s;

			void* pngData = stbi__do_png(
				&p, &data->width, &data->height, 
				(int32*)&data->channels,
				forceChannelsCount, ri);

			data->depth = p.depth;
			data->bitsPerChannel = ri->bits_per_channel;
			data->colorSpace = ri->channel_order == STBI_ORDER_RGB
				? Image::ColorSpace::RGB
				: Image::ColorSpace::BGR; // ? TODO

			return pngData;
		}

		static void* loadJpeg(
			stbi__context* s,
			Image::ImageData* data, 
			ushort forceChannelsCount)
		{
			stbi__jpeg* jpeg = (stbi__jpeg*)stbi__malloc(sizeof(stbi__jpeg));
			if (!jpeg)
				return stbi__errpuc("outofmem", "Out of memory");

			memset(jpeg, 0, sizeof(stbi__jpeg));
			jpeg->s = s;

			stbi__setup_jpeg(jpeg);

			uchar* result = load_jpeg_image(
				jpeg, &data->width, &data->height,
				(int32*)&data->channels, forceChannelsCount);

			free(jpeg);
			return result;
		}

		static void* loadBmp(
			stbi__context* s, 
			Image::ImageData* data, 
			ushort forceChannelsCount)
		{
			stbi_uc* out;

			unsigned int mr = 0, mg = 0, mb = 0, ma = 0, all_a;

			stbi_uc pal[256][4];

			int psize = 0, i, j, width;
			int flip_vertically, pad, target;

			stbi__bmp_data info;

			info.all_a = 255;

			if (stbi__bmp_parse_header(s, &info) == NULL)
				return NULL; // error code already set

			flip_vertically = ((int)s->img_y) > 0;
			s->img_y = abs((int)s->img_y);

			if (s->img_y > STBI_MAX_DIMENSIONS) 
				return stbi__errpuc("too large", "Very large image (corrupt?)");

			if (s->img_x > STBI_MAX_DIMENSIONS)
				return stbi__errpuc("too large", "Very large image (corrupt?)");

			mr = info.mr;
			mg = info.mg;
			mb = info.mb;
			ma = info.ma;

			all_a = info.all_a;

			if (info.hsz == 12) {
				if (info.bpp < 24)
					psize = (info.offset - info.extra_read - 24) / 3;
			}
			else {
				if (info.bpp < 16)
					psize = (info.offset - info.extra_read - info.hsz) >> 2;
			}
			if (psize == 0) {
				// accept some number of extra bytes after the header, but if the offset points either to before
				// the header ends or implies a large amount of extra data, reject the file as malformed
				int bytes_read_so_far = s->callback_already_read + (int)(s->img_buffer - s->img_buffer_original);
				int header_limit = 1024; // max we actually read is below 256 bytes currently.
				int extra_data_limit = 256 * 4; // what ordinarily goes here is a palette; 256 entries*4 bytes is its max size.
				if (bytes_read_so_far <= 0 || bytes_read_so_far > header_limit) {
					return stbi__errpuc("bad header", "Corrupt BMP");
				}
				// we established that bytes_read_so_far is positive and sensible.
				// the first half of this test rejects offsets that are either too small positives, or
				// negative, and guarantees that info.offset >= bytes_read_so_far > 0. this in turn
				// ensures the number computed in the second half of the test can't overflow.
				if (info.offset < bytes_read_so_far || info.offset - bytes_read_so_far > extra_data_limit) {
					return stbi__errpuc("bad offset", "Corrupt BMP");
				}
				else {
					stbi__skip(s, info.offset - bytes_read_so_far);
				}
			}

			if (info.bpp == 24 && ma == 0xff000000)
				s->img_n = 3;
			else
				s->img_n = ma ? 4 : 3;
			if (forceChannelsCount && forceChannelsCount >= 3) // we can directly decode 3 or 4
				target = forceChannelsCount;
			else
				target = s->img_n; // if they want monochrome, we'll post-convert

			// sanity-check size
			if (!stbi__mad3sizes_valid(target, s->img_x, s->img_y, 0))
				return stbi__errpuc("too large", "Corrupt BMP");

			out = (stbi_uc*)stbi__malloc_mad3(target, s->img_x, s->img_y, 0);
			if (!out) return stbi__errpuc("outofmem", "Out of memory");
			if (info.bpp < 16) {
				int z = 0;
				if (psize == 0 || psize > 256) { STBI_FREE(out); return stbi__errpuc("invalid", "Corrupt BMP"); }
				for (i = 0; i < psize; ++i) {
					pal[i][2] = stbi__get8(s);
					pal[i][1] = stbi__get8(s);
					pal[i][0] = stbi__get8(s);
					if (info.hsz != 12) stbi__get8(s);
					pal[i][3] = 255;
				}
				stbi__skip(s, info.offset - info.extra_read - info.hsz - psize * (info.hsz == 12 ? 3 : 4));
				if (info.bpp == 1) width = (s->img_x + 7) >> 3;
				else if (info.bpp == 4) width = (s->img_x + 1) >> 1;
				else if (info.bpp == 8) width = s->img_x;
				else { STBI_FREE(out); return stbi__errpuc("bad bpp", "Corrupt BMP"); }
				pad = (-width) & 3;
				if (info.bpp == 1) {
					for (j = 0; j < (int)s->img_y; ++j) {
						int bit_offset = 7, v = stbi__get8(s);
						for (i = 0; i < (int)s->img_x; ++i) {
							int color = (v >> bit_offset) & 0x1;
							out[z++] = pal[color][0];
							out[z++] = pal[color][1];
							out[z++] = pal[color][2];
							if (target == 4) out[z++] = 255;
							if (i + 1 == (int)s->img_x) break;
							if ((--bit_offset) < 0) {
								bit_offset = 7;
								v = stbi__get8(s);
							}
						}
						stbi__skip(s, pad);
					}
				}
				else {
					for (j = 0; j < (int)s->img_y; ++j) {
						for (i = 0; i < (int)s->img_x; i += 2) {
							int v = stbi__get8(s), v2 = 0;
							if (info.bpp == 4) {
								v2 = v & 15;
								v >>= 4;
							}
							out[z++] = pal[v][0];
							out[z++] = pal[v][1];
							out[z++] = pal[v][2];
							if (target == 4) out[z++] = 255;
							if (i + 1 == (int)s->img_x) break;
							v = (info.bpp == 8) ? stbi__get8(s) : v2;
							out[z++] = pal[v][0];
							out[z++] = pal[v][1];
							out[z++] = pal[v][2];
							if (target == 4) out[z++] = 255;
						}
						stbi__skip(s, pad);
					}
				}
			}
			else {
				int rshift = 0, gshift = 0, bshift = 0, ashift = 0, rcount = 0, gcount = 0, bcount = 0, acount = 0;
				int z = 0;
				int easy = 0;
				stbi__skip(s, info.offset - info.extra_read - info.hsz);
				if (info.bpp == 24) width = 3 * s->img_x;
				else if (info.bpp == 16) width = 2 * s->img_x;
				else /* bpp = 32 and pad = 0 */ width = 0;
				pad = (-width) & 3;
				if (info.bpp == 24) {
					easy = 1;
				}
				else if (info.bpp == 32) {
					if (mb == 0xff && mg == 0xff00 && mr == 0x00ff0000 && ma == 0xff000000)
						easy = 2;
				}
				if (!easy) {
					if (!mr || !mg || !mb) { STBI_FREE(out); return stbi__errpuc("bad masks", "Corrupt BMP"); }
					// right shift amt to put high bit in position #7
					rshift = stbi__high_bit(mr) - 7; rcount = stbi__bitcount(mr);
					gshift = stbi__high_bit(mg) - 7; gcount = stbi__bitcount(mg);
					bshift = stbi__high_bit(mb) - 7; bcount = stbi__bitcount(mb);
					ashift = stbi__high_bit(ma) - 7; acount = stbi__bitcount(ma);
					if (rcount > 8 || gcount > 8 || bcount > 8 || acount > 8) { STBI_FREE(out); return stbi__errpuc("bad masks", "Corrupt BMP"); }
				}
				for (j = 0; j < (int)s->img_y; ++j) {
					if (easy) {
						for (i = 0; i < (int)s->img_x; ++i) {
							unsigned char a;
							out[z + 2] = stbi__get8(s);
							out[z + 1] = stbi__get8(s);
							out[z + 0] = stbi__get8(s);
							z += 3;
							a = (easy == 2 ? stbi__get8(s) : 255);
							all_a |= a;
							if (target == 4) out[z++] = a;
						}
					}
					else {
						int bpp = info.bpp;
						for (i = 0; i < (int)s->img_x; ++i) {
							stbi__uint32 v = (bpp == 16 ? (stbi__uint32)stbi__get16le(s) : stbi__get32le(s));
							unsigned int a;
							out[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mr, rshift, rcount));
							out[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mg, gshift, gcount));
							out[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mb, bshift, bcount));
							a = (ma ? stbi__shiftsigned(v & ma, ashift, acount) : 255);
							all_a |= a;
							if (target == 4) out[z++] = STBI__BYTECAST(a);
						}
					}
					stbi__skip(s, pad);
				}
			}

			// if alpha channel is all 0s, replace with all 255s
			if (target == 4 && all_a == 0)
				for (i = 4 * s->img_x * s->img_y - 1; i >= 0; i -= 4)
					out[i] = 255;

			if (flip_vertically) {
				stbi_uc t;
				for (j = 0; j < (int)s->img_y >> 1; ++j) {
					stbi_uc* p1 = out + j * s->img_x * target;
					stbi_uc* p2 = out + (s->img_y - 1 - j) * s->img_x * target;
					for (i = 0; i < (int)s->img_x * target; ++i) {
						t = p1[i]; p1[i] = p2[i]; p2[i] = t;
					}
				}
			}

			if (forceChannelsCount && forceChannelsCount != target) {
				out = stbi__convert_format(out, target, forceChannelsCount, s->img_x, s->img_y);
				if (out == NULL) 
					return out; // stbi__convert_format frees input on failure
			}

			data->width = s->img_x;
			data->height = s->img_y;

			if (forceChannelsCount)
				data->channels = s->img_n;

			data->bitsPerChannel = info.bpp;

			return out;
		}
		static void* loadImageMain(
			::stbi__context* context,
			Image::ImageData* data,
			int32 forceChannelsCount,
			::stbi__result_info* ri,
			int32 bitsPerChannel)
		{
			measureExecutionTime("base::images::Image::loadImageMain")
				memset(ri, 0, sizeof(*ri));

			ri->bits_per_channel = 8;
			ri->channel_order = STBI_ORDER_RGB;
			ri->num_channels = 0;

			if (stbi__png_test(context)) {
				data->imageExtension = "png";
				return loadPng(context, data, forceChannelsCount, ri);
			}

			if (stbi__bmp_test(context)) {
				data->imageExtension = "bmp";
				return loadBmp(context, data, forceChannelsCount);
			}

			if (stbi__jpeg_test(context)) {
				data->imageExtension = "jpeg";
				return loadJpeg(context, data, forceChannelsCount);
			}

			return stbi__errpuc("unknown image type", "Image not of any known type, or corrupt");
		}

		static uchar* loadAndPostprocess8Bit(
			::stbi__context* context,
			Image::ImageData* data,
			int32 forceChannelsCount)
		{
			::stbi__result_info resultInfo;
			void* result = loadImageMain(context, data, forceChannelsCount, &resultInfo, 8);

			if (result == NULL)
				return NULL;

			STBI_ASSERT(resultInfo.bits_per_channel == 8 || resultInfo.bits_per_channel == 16);

			if (resultInfo.bits_per_channel != 8) {
				measureExecutionTime("base::images::Image::loadAndPostprocess8Bit - resultInfo.bits_per_channel != 8: ")
					result = stbi__convert_16_to_8((stbi__uint16*)result,
						data->width, data->height,
						forceChannelsCount == 0
						? data->channels
						: forceChannelsCount);

				resultInfo.bits_per_channel = 8;
			}

			if (stbi__vertically_flip_on_load) {
				measureExecutionTime("base::images::Image::loadAndPostprocess8Bit - stbi__vertically_flip_on_load: ")
					int channels = forceChannelsCount ? forceChannelsCount : data->channels;
				stbi__vertical_flip(result, data->width, data->height, channels * sizeof(stbi_uc));
			}

			return (uchar*)result;
		}

	} // namespace

	Image::Image() :
		_data(new ImageData())
	{}

	Image::Image(const char* path) :
		_data(new ImageData())
	{
		loadFromFile(path);
	}

	Image::Image(Image&& image) noexcept :
		_data(image._data)
	{}

	Image::Image(const IntegralImage& image) {

	}

	Image::Image(::uchar* data) :
		_data(new ImageData())
	{
		_data->data = data;
	}

	Image::Image(const Size<int32>& size) :
		_data(new ImageData())
	{
		_data->width = size.width();
		_data->height = size.height();
	}

	Image::Image(int32 width, int32 height) :
		_data(new ImageData())
	{
		_data->width = width;
		_data->height = height;
	}

	Image::Image(
		uchar* data,
		int32 width, int32 height
	) :
		_data(new ImageData())
	{
		_data->data = data;

		_data->width = width;
		_data->height = height;
	}

	Image::Image(
		uchar* data,
		const Size<int32>& size
	) :
		_data(new ImageData())

	{
		_data->data = data;

		_data->width = size.width();
		_data->height = size.height();
	}

#ifdef LIB_BASE_ENABLE_QT
	Image::Image(QImage&& image) :
		_data(new ImageData())
	{
		_data->data = image.bits();
		_data->depth = image.depth();

		_data->sizeInBytes = image.sizeInBytes();
		_data->devicePixelRatio = image.devicePixelRatio();

		_data->channels = getChannelsCountByFormat(image);

		_data->width = image.width();
		_data->height = image.height();
	}
#endif

	bool Image::operator==(const Image& other) {
		return isEqual(other);
	}

	bool Image::operator!=(const Image& other) {
		return !isEqual(other);
	}

	void Image::loadFromData(const uchar* data, sizetype length) {
		AssertLog(data != nullptr, "base::images::Image::loadFromData: Given data is null");
		AssertLog(length > 0, "base::images::Image::loadFromData: Cannot load from memory width length <= 0");

		_data->data = stbi_load_from_memory(
			data, length, &_data->width,
			&_data->height, (int32*)&_data->channels, kForceImageChannels);

		AssertLog(_data->data != nullptr, "base::images::Image::loadFromData: Cannot load image from memory");

		_data->sizeInBytes = length;
		_data->bytesPerLine = recountBytesPerLine(_data);
	}

	void Image::loadFromFile(const char* path) {
		_data->path = path;
		readImage(_data);

		AssertLog(_data->data != nullptr, 
			std::string("base::images::Image::loadFromFile: Cannot load image from file: "
			+ std::string(path)).c_str());

		_data->bytesPerLine = recountBytesPerLine(_data);
		qDebug() << "_data->sizeInBytes: " << _data->sizeInBytes << "_data->w&h: " << _data->width
				 << _data->height << "_data->bytesPerLine: " << _data->bytesPerLine;
	}

	Image::~Image() {
		if (isNull() == false)
			stbi_image_free(_data->data);

		if (_data != nullptr)
			delete base::take(_data);
	}

	Image Image::convertToColorSpace(ColorSpace space) const {
		return Image();
	}

	void Image::resize(int32 width, int32 height) {
		/*stbi_resize_uint8(
			_data->data, _data->width, _data->height, 0,
			_data->data, width, height, 0, _data->channels);*/

		_data->width = width;
		_data->height = height;

		_data->sizeInBytes = sizeof(*_data->data);
		_data->bytesPerLine = recountBytesPerLine(_data);
	}

	void Image::resize(Size<int32> size) {
		return resize(size.width(), size.height());
	}

	bool Image::isJpeg(const char* ext) {
		return (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0);
	}

	bool Image::isPng(const char* ext) {
		return (strcmp(ext, "png") == 0);
	}

	bool Image::isBmp(const char* ext) {
		return (strcmp(ext, "bmp") == 0);
	}

	bool Image::isFormatSupported(const char* format) {
		return isJpeg(format) || isBmp(format) || isPng(format);
	}

	bool Image::isFormatsEqual(
		const char* firstFormat,
		const char* secondFormat) 
	{
		if (isJpeg(firstFormat) && isJpeg(secondFormat))
			return true;
		
		return (strcmp(firstFormat, secondFormat) == 0);
	} 


	void Image::convertToFormat(const char* format) {
		if (isFormatsEqual(_data->imageExtension, format))
			return;

		if (isPng(_data->imageExtension)) {
			if (isJpeg(format)) {
				stbi_write_jpg_to_func([](void* context, void* data, int size) {
					memcpy(((ImageData*)context)->data + ((ImageData*)context)->dataLength, data, size);
					((ImageData*)context)->dataLength += size;
					},
					_data, _data->width, _data->height,
					_data->channels, _data->data, _jpegQuality);
			}
			else if (isBmp(format)) {
				stbi_write_bmp_to_func([](void* context, void* data, int size) {
					memcpy(((ImageData*)context)->data + ((ImageData*)context)->dataLength, data, size);
					((ImageData*)context)->dataLength += size;
					},
					_data, _data->width, _data->height,
					_data->channels, _data->data);
			}
		}

		else if (isJpeg(_data->imageExtension)) {
			if (isPng(format)) {
				stbi_write_png_to_mem(
					_data->data, _data->width * _data->depth,
					_data->width, _data->height,
					_data->channels, &_data->sizeInBytes);
			}
			else if (isBmp(format)) {
				stbi_write_bmp_to_func([](void* context, void* data, int size) {
					memcpy(((ImageData*)context)->data + ((ImageData*)context)->dataLength, data, size);
					((ImageData*)context)->dataLength += size;
					},
					_data, _data->width, _data->height,
					_data->channels, _data->data);
			}
		}

		else if (isBmp(_data->imageExtension)) {
			if (isPng(format)) {
				stbi_write_png_to_mem(
					_data->data, _data->width * _data->depth,
					_data->width, _data->height,
					_data->channels, &_data->sizeInBytes);
			}
			else if (isJpeg(format)) {
				stbi_write_jpg_to_func([](void* context, void* data, int size) {
					memcpy(((ImageData*)context)->data + ((ImageData*)context)->dataLength, data, size);
					((ImageData*)context)->dataLength += size;
					},
					_data, _data->width, _data->height,
					_data->channels, _data->data, _jpegQuality);
			}
		}

		_data->imageExtension = format;
	}

	const char* Image::format() const noexcept {
		return _data->imageExtension;
	}

	void Image::save(const char* path) {
		AssertLog(!isNull(), std::string("base::images::Image::save: Cannot save a null image to path - " + std::string(path)).c_str());
		writeImageToFile(_data, path);
	}

	void Image::setJpegQuality(ushort quality) {
		_jpegQuality = quality;
	}

	ushort Image::jpegQuality() const noexcept {
		return _jpegQuality;
	}

	Rect<int32> Image::rect() const noexcept {
		return Rect<int32>(0, 0, _data->width, _data->height);
	}

	Size<int32> Image::size() const noexcept {
		return Size<int32>(_data->width, _data->height);
	}

	int32 Image::width() const noexcept {
		return _data->width;
	}

	int32 Image::height() const noexcept {
		return _data->height;
	}

	int32 Image::bytesPerLine() const noexcept {
		return _data->bytesPerLine;
	}

	uchar* Image::bytesData() {
		return _data->data;
	}

	Image::ImageData* Image::data_ptr() {
		return _data;
	}

	uchar* Image::scanLine(int i) {
		if (!_data)
			return nullptr;

		return _data->data + i * _data->bytesPerLine;
	}

	const uchar* Image::scanLine(int i) const
	{
		if (!_data)
			return nullptr;

		Assert(i >= 0 && i < height());
		return _data->data + i * _data->bytesPerLine;
	}

	bool Image::isNull() const noexcept {
		return _data == nullptr
			|| (_data->data == nullptr || _data->sizeInBytes <= 0)
			|| (_data->width <= 0 || _data->height <= 0);
	}

	Rgb Image::pixel(int x, int y) const
	{
		if (!_data || x < 0 || x >= _data->width || y < 0 || y >= _data->height) {
			qWarning("QImage::pixel: coordinate (%d,%d) out of range", x, y);
			return -1;
		}

		return -1;
	}

	bool Image::isEqual(const Image& other) const {
		return _data->bytesPerLine == other._data->bytesPerLine
			&& _data->data == other._data->data
			&& _data->depth == other._data->depth
			&& _data->devicePixelRatio == other._data->devicePixelRatio
			&& _data->sizeInBytes == other._data->sizeInBytes
			&& _data->width == other._data->width
			&& _data->height == other._data->height;
	}

	int32 Image::recountBytesPerLine(ImageData* data)
	{
		qDebug() << "base::images::Image::recountBytesPerLine: depth - " << data->depth;
		int32 invalid = -1;

		if (data->height <= 0)
			return invalid;

		auto _bytesPerLine = int32(0);

		if (MultiplyOverflow(data->width, data->depth, &_bytesPerLine))
			return invalid;
		if (AdditionOverflow(_bytesPerLine, 31, &_bytesPerLine))
			return invalid;

		_bytesPerLine = (_bytesPerLine >> 5) << 2;    // can't overflow

		auto dummy = sizetype(0);
		if (MultiplyOverflow(data->height, sizetype(sizeof(uchar*)), &dummy))
			return invalid;

		if (data->width > (INT_MAX - 31) / data->depth)
			return invalid;

		return _bytesPerLine;
	}

	void Image::readImage(
		ImageData* data,
		int32 forceChannelsCount)
	{
		if (data == nullptr || data->path.has_value() == false)
			return;

		FILE* file = stbi__fopen(data->path.value(), "rb");
		AssertLog(file != nullptr, "base::images::Image::readImage: Cannot fopen. Unable to open file");

		data->data = loadImage(file, data, forceChannelsCount);

		data->sizeInBytes = ftell(file);
		fclose(file);
	}


	uchar* Image::loadImage(
		FILE* file,
		ImageData* data,
		int32 forceChannelsCount)
	{
		::stbi__context context;
		stbi__start_file(&context, file);

		uchar* result = loadAndPostprocess8Bit(
			&context, data, forceChannelsCount);

		if (result) {
			fseek(file, -(int)(context.img_buffer_end - context.img_buffer), SEEK_CUR);
			data->dataLength = strlen((const char*)result) + 1;
		}

		return result;
	}

	void Image::writeImageToFile(
		ImageData* data,
		const std::string& path)
	{
		int32 success = 0;
		std::string outputImageFormat = getExtensionFromPath(path);

		AssertLog(isFormatSupported(outputImageFormat.c_str()) != false,
			std::string("base::images::Image::writeImageToFile: Given format: \"" 
				+ outputImageFormat + "\"" + std::string(" is not supported")).c_str());

		convertToFormat(outputImageFormat.c_str());

		if (isPng(data->imageExtension))
			success = stbi_write_png(
				path.c_str(), data->width, data->height,
				data->channels, data->data, data->width * data->channels);

		else if (isJpeg(data->imageExtension))
			success = stbi_write_jpg(
				path.c_str(), data->width, data->height,
				data->channels, data->data, _jpegQuality);

		else if (isBmp(data->imageExtension))
			success = stbi_write_bmp(
				path.c_str(), data->width, data->height,
				data->channels, data->data);
		
		AssertLog(success != 0, "base::images::Image::writeImageToFile: Error while writing");
	}

	std::string Image::getExtensionFromPath(const std::string& path) {
		std::regex regex(R"(\.(bmp|jpe?g|png)(?=[^.]*$))", std::regex::icase);
		std::smatch match;

		if (std::regex_search(path, match, regex)) {
			std::string result = match[1].str().erase(0, 0);
			return result;
		}

		size_t pos = path.rfind('.');
		if (pos != std::string::npos)
			return path.substr(pos);
	
		return "";
	}
} // namespace base::images