#include <base/images/ImagesUtility.h>
#include <base/Utility.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE2_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <base/images/stb/StbImage.h>
#include <base/images/stb/StbImageWrite.h>
#include <base/images/stb/StbImageResize.h>

#include <iostream>
#include <regex>

#define JPEG_FORMAT_DEPTH 24

namespace base::images::Utility {
#ifdef LIB_BASE_ENABLE_QT
    int GetChannelsCountByFormat(const QImage& image) {
        auto format = image.format();

        switch (format) {
            case QImage::Format_Mono:
            case QImage::Format_MonoLSB:
                return 1;

            case QImage::Format_Grayscale8:
                return 1;

            case QImage::Format_Indexed8:
                return 1;

            case QImage::Format_RGB32:
            case QImage::Format_ARGB32:
            case QImage::Format_ARGB32_Premultiplied:
            case QImage::Format_RGBA8888:
            case QImage::Format_RGBA8888_Premultiplied:

            case QImage::Format_RGB888:
            case QImage::Format_BGR888:
                return 3;
        }

        return -1;
    }

    [[nodiscard]] int GetChannelsCountByBytes(const QImage& image) {
        const auto width = image.width();
        const auto bytesPerLine = image.bytesPerLine();

        if (width <= 0 || bytesPerLine <= 0)
            return -1;

        const auto bytesPerPixel = bytesPerLine / width;

        switch (bytesPerPixel) {
            case 1:  return 1; // Grayscale, Mono
            case 3:  return 3; // RGB
            case 4: return 4;  // RGBA
        }

        return -1;
    }
#endif
	bool IsJpeg(const char* ext) {
		return strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0
			|| strcmp(ext, "jpe") == 0;
	}

	bool IsPng(const char* ext) {
		return (strcmp(ext, "png") == 0);
	}

	bool IsBmp(const char* ext) {
		return (strcmp(ext, "bmp") == 0);
	}

	bool IsFormatSupported(const char* format) {
		return IsJpeg(format) || IsBmp(format) || IsPng(format);
	}

	bool IsFormatsEqual(
		const char* firstFormat,
		const char* secondFormat)
	{
		if (IsJpeg(firstFormat) && IsJpeg(secondFormat))
			return true;

		return (strcmp(firstFormat, secondFormat) == 0);
	}

	void StbiWrite(
		void* context, 
		void* data, 
		int size)
	{
		memcpy(((ImageData*)context)->data + ((ImageData*)context)->dataLength, data, size);
		((ImageData*)context)->dataLength += size;
	};

#ifndef LIB_BASE_IMAGES_NO_GIF
	// 
#endif

#ifndef LIB_BASE_IMAGES_NO_PNG
	[[nodiscard]] void* loadPng(
		stbi__context* s,
		ImageData* data,
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
			? ColorSpace::RGB
			: ColorSpace::BGR; // ? TODO

		return pngData;
	}

#endif

#ifndef LIB_BASE_IMAGES_NO_JPEG

	static void* loadJpeg(
		stbi__context* s,
		ImageData* data,
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

		data->depth = JPEG_FORMAT_DEPTH;
		data->jpegQuality = kDefaultStbiJpegQuality;

		free(jpeg);
		return result;
	}
#endif

#ifndef LIB_BASE_IMAGES_NO_BMP
	static void* loadBmp(
		stbi__context* s,
		ImageData* data,
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

			if (bytes_read_so_far <= 0 || bytes_read_so_far > header_limit)
				return stbi__errpuc("bad header", "Corrupt BMP");
			
			// we established that bytes_read_so_far is positive and sensible.
			// the first half of this test rejects offsets that are either too small positives, or
			// negative, and guarantees that info.offset >= bytes_read_so_far > 0. this in turn
			// ensures the number computed in the second half of the test can't overflow.
			if (info.offset < bytes_read_so_far || info.offset - bytes_read_so_far > extra_data_limit)
				return stbi__errpuc("bad offset", "Corrupt BMP");
			else
				stbi__skip(s, info.offset - bytes_read_so_far);
			
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

		if (!out) 
			return stbi__errpuc("outofmem", "Out of memory");

		if (info.bpp < 16) {
			int z = 0;

			if (psize == 0 || psize > 256) { 
				STBI_FREE(out); 
				return stbi__errpuc("invalid", "Corrupt BMP"); 
			}

			for (i = 0; i < psize; ++i) {
				pal[i][2] = stbi__get8(s);
				pal[i][1] = stbi__get8(s);
				pal[i][0] = stbi__get8(s);

				if (info.hsz != 12) stbi__get8(s);

				pal[i][3] = 255;
			}

			stbi__skip(s, info.offset - info.extra_read - 
				info.hsz - psize * 
				(info.hsz == 12 ? 3 : 4));

			if (info.bpp == 1) 
				width = (s->img_x + 7) >> 3;

			else if (info.bpp == 4) 
				width = (s->img_x + 1) >> 1;

			else if (info.bpp == 8) 
				width = s->img_x;

			else { 
				STBI_FREE(out); 
				return stbi__errpuc("bad bpp", "Corrupt BMP");
			}

			pad = (-width) & 3;

			if (info.bpp == 1) {
				for (j = 0; j < (int)s->img_y; ++j) {
					int bit_offset = 7, v = stbi__get8(s);
					for (i = 0; i < (int)s->img_x; ++i) {

						int color = (v >> bit_offset) & 0x1;

						out[z++] = pal[color][0];
						out[z++] = pal[color][1];
						out[z++] = pal[color][2];

						if (target == 4) 
							out[z++] = 255;

						if (i + 1 == (int)s->img_x)
							break;

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

						if (target == 4)
							out[z++] = 255;

						if (i + 1 == (int)s->img_x) 
							break;

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
			int rshift = 0, gshift = 0, bshift = 0, ashift = 0, 
				rcount = 0, gcount = 0, bcount = 0, acount = 0;

			int z = 0;
			int easy = 0;

			stbi__skip(s, info.offset - info.extra_read - info.hsz);

			if (info.bpp == 24) 
				width = 3 * s->img_x;

			else if (info.bpp == 16) 
				width = 2 * s->img_x;

			else /* bpp = 32 and pad = 0 */ 
				width = 0;

			pad = (-width) & 3;

			if (info.bpp == 24)
				easy = 1;
			else if (info.bpp == 32)
				if (mb == 0xff && mg == 0xff00 && mr == 0x00ff0000 && ma == 0xff000000)
					easy = 2;
			
			if (!easy) {
				if (!mr || !mg || !mb) { 
					STBI_FREE(out); 
					return stbi__errpuc("bad masks", "Corrupt BMP"); 
				}
				// right shift amt to put high bit in position #7
				rshift = stbi__high_bit(mr) - 7; rcount = stbi__bitcount(mr);
				gshift = stbi__high_bit(mg) - 7; gcount = stbi__bitcount(mg);

				bshift = stbi__high_bit(mb) - 7; bcount = stbi__bitcount(mb);
				ashift = stbi__high_bit(ma) - 7; acount = stbi__bitcount(ma);

				if (rcount > 8 || gcount > 8 || bcount > 8 || acount > 8) {
					STBI_FREE(out);
					return stbi__errpuc("bad masks", "Corrupt BMP");
				}
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

						if (target == 4) 
							out[z++] = a;
					}
				}
				else {
					int bpp = info.bpp;

					for (i = 0; i < (int)s->img_x; ++i) {
						stbi__uint32 v = (bpp == 16 
							? (stbi__uint32)stbi__get16le(s) 
							: stbi__get32le(s));

						unsigned int a;

						out[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mr, rshift, rcount));
						out[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mg, gshift, gcount));
						out[z++] = STBI__BYTECAST(stbi__shiftsigned(v & mb, bshift, bcount));

						a = (ma ? stbi__shiftsigned(v & ma, ashift, acount) : 255);
						all_a |= a;

						if (target == 4) 
							out[z++] = STBI__BYTECAST(a);
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

				for (i = 0; i < (int)s->img_x * target; ++i)
					t = p1[i]; p1[i] = p2[i]; p2[i] = t;
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
#endif

	static void* loadImageMain(
		::stbi__context* context,
		ImageData* data,
		int32 forceChannelsCount,
		::stbi__result_info* ri,
		int32 bitsPerChannel)
	{
		measureExecutionTime("base::images::Image::loadImageMain")
		memset(ri, 0, sizeof(*ri));

		ri->bits_per_channel = 8;
		ri->channel_order = STBI_ORDER_RGB;
		ri->num_channels = 0;

#ifndef LIB_BASE_IMAGES_NO_PNG
		if (stbi__png_test(context)) {
			data->imageExtension = "png";
			return loadPng(context, data, forceChannelsCount, ri);
		}
#endif
#ifndef LIB_BASE_IMAGES_NO_BMP
		if (stbi__bmp_test(context)) {
			data->imageExtension = "bmp";
			return loadBmp(context, data, forceChannelsCount);
		}
#endif
#ifndef LIB_BASE_IMAGES_NO_JPEG
		if (stbi__jpeg_test(context)) {
			data->imageExtension = "jpeg";
			return loadJpeg(context, data, forceChannelsCount);
		}
#endif

		return stbi__errpuc("unknown image type", "Image not of any known or supported type, or corrupt");
	}

	static uchar* loadAndPostprocess8Bit(
		::stbi__context* context,
		ImageData* data,
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

			int channels = forceChannelsCount 
				? forceChannelsCount 
				: data->channels;

			stbi__vertical_flip(result, data->width, data->height, channels * sizeof(stbi_uc));
		}

		return (uchar*)result;
	}

	uchar* loadImage(
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

	void ReadImage(
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

	int32 CountBytesPerLine(ImageData* data)
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

	std::string GetExtensionFromPath(const std::string& path) {
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

	void ConvertToFormat(
		ImageData* _data, 
		const char* format) 
	{
		if (IsFormatsEqual(_data->imageExtension, format))
			return;

		if (IsPng(_data->imageExtension)) {
			if (IsJpeg(format)) {
				stbi_write_jpg_to_func([](void* context, void* data, int size) {
					memcpy(((ImageData*)context)->data + ((ImageData*)context)->dataLength, data, size);
					((ImageData*)context)->dataLength += size;
					},
					_data, _data->width, _data->height,
					_data->channels, _data->data, _data->jpegQuality.has_value() 
						? _data->jpegQuality.value() 
						: kDefaultStbiJpegQuality);
			}
			else if (IsBmp(format)) {
				stbi_write_bmp_to_func([](void* context, void* data, int size) {
					memcpy(((ImageData*)context)->data + ((ImageData*)context)->dataLength, data, size);
					((ImageData*)context)->dataLength += size;
					},
					_data, _data->width, _data->height,
					_data->channels, _data->data);
			}
		}

		else if (IsJpeg(_data->imageExtension)) {
			if (IsPng(format)) {
				stbi_write_png_to_mem(
					_data->data,  _data->width * _data->channels,	
					_data->width, _data->height,
					_data->channels, &_data->sizeInBytes);
			}
			else if (IsBmp(format)) {
				stbi_write_bmp_to_func([](void* context, void* data, int size) {
					memcpy(((ImageData*)context)->data + ((ImageData*)context)->dataLength, data, size);
					((ImageData*)context)->dataLength += size;
					},
					_data, _data->width, _data->height,
					_data->channels, _data->data);
			}
		}

		_data->imageExtension = format;
	}


	void WriteImageToFile(
		ImageData* data,
		const std::string& path)
	{
		int32 success = 0;
		std::string outputImageFormat = GetExtensionFromPath(path);

		AssertLog(IsFormatSupported(outputImageFormat.c_str()) != false,
			std::string("base::images::Image::writeImageToFile: Given format: \""
				+ outputImageFormat + "\"" + std::string(" is not supported")).c_str());

		ConvertToFormat(data, outputImageFormat.c_str());

		if (IsPng(data->imageExtension))
			success = stbi_write_png(
				path.c_str(), data->width, data->height,
				data->channels, data->data, data->width * data->channels);

		AssertLog(success != 0, "base::images::Image::writeImageToFile: Error while writing");
	}

} // namespace base::images