#include <base/images/ImageReader.h>
#include <base/Utility.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE2_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include <base/images/stb/StbImage.h>
#include <base/images/stb/StbImageWrite.h>
#include <base/images/stb/StbImageResize.h>

#include <base/images/ImagesImage.h>
#include <base/images/ImagesUtility.h>

#include <base/images/formats/BmpHandler.h>
#include <base/images/formats/JpegHandler.h>

#include <base/images/formats/PngHandler.h>

#define JPEG_FORMAT_DEPTH 24
#define STBI_AUTO_DETECT_CHANNELS_COUNT 0


namespace base::images {
	class ImageReaderPrivate {
	public:
	#ifndef LIB_BASE_IMAGES_NO_GIF
			// 
	#endif

	#ifndef LIB_BASE_IMAGES_NO_PNG
		[[nodiscard]] int parsePng(
			stbi__png* z, 
			int scan, int req_comp);

		[[nodiscard]] void* doPng(
			stbi__png* p,
			ImageData* data,
			stbi__result_info* ri);

		[[nodiscard]] void* loadPng(
			stbi__context* s,
			ImageData* data,
			stbi__result_info* ri);
	#endif

	#ifndef LIB_BASE_IMAGES_NO_JPEG
		stbi_uc* loadJpegImage(
			stbi__jpeg* z,
			ImageData* data);

		void* loadJpeg(
			stbi__context* s,
			ImageData* data);
	#endif

	#ifndef LIB_BASE_IMAGES_NO_BMP
		void* loadBmp(
			stbi__context* s,
			ImageData* data);
	#endif

		void* loadImageMain(
			stbi__context* context,
			ImageData* data,
			stbi__result_info* ri,
			int32 bitsPerChannel);

		uchar* loadAndPostprocess8Bit(
			stbi__context* context,
			ImageData* data);

		uchar* loadImage(
			FILE* file,
			ImageData* data);

		void ReadImage(ImageData* data);

		const char* _path = nullptr;
	};

#ifndef LIB_BASE_IMAGES_NO_PNG
	int ImageReaderPrivate::parsePng(stbi__png* z, int scan, int req_comp)
	{
		stbi_uc palette[1024], pal_img_n = 0;
		stbi_uc has_trans = 0, tc[3] = { 0 };

		stbi__uint16 tc16[3];
		stbi__uint32 ioff = 0, idata_limit = 0, i, pal_len = 0;

		int first = 1, k, interlace = 0, color = 0, is_iphone = 0;
		stbi__context* s = z->s;

		z->expanded = NULL;
		z->idata = NULL;
		z->out = NULL;

		if (!stbi__check_png_header(s)) 
			return 0;

		if (scan == STBI__SCAN_type)
			return 1;

		for (;;) {
			stbi__pngchunk c = stbi__get_chunk_header(s);

			switch (c.type) {
				case STBI__PNG_TYPE('C', 'g', 'B', 'I'):
					is_iphone = 1;
					stbi__skip(s, c.length);

					break;

				case STBI__PNG_TYPE('I', 'H', 'D', 'R'): {
					int comp, filter;

					if (!first) 
						return stbi__err("multiple IHDR", "Corrupt PNG");

					first = 0;

					if (c.length != 13)
						return stbi__err("bad IHDR len", "Corrupt PNG");

					s->img_x = stbi__get32be(s);
					s->img_y = stbi__get32be(s);

					if (s->img_y > STBI_MAX_DIMENSIONS) 
						return stbi__err("too large", "Very large image (corrupt?)");

					if (s->img_x > STBI_MAX_DIMENSIONS) 
						return stbi__err("too large", "Very large image (corrupt?)");

					z->depth = stbi__get8(s);  
					if (z->depth != 1 && z->depth != 2 && z->depth 
						!= 4 && z->depth != 8 && z->depth != 16)  
						return stbi__err("1/2/4/8/16-bit only", "PNG not supported: 1/2/4/8/16-bit only");

					color = stbi__get8(s);  
					if (color > 6)         
						return stbi__err("bad ctype", "Corrupt PNG");

					if (color == 3 && z->depth == 16)                 
						return stbi__err("bad ctype", "Corrupt PNG");

					if (color == 3) 
						pal_img_n = 3; 
					else if (color & 1)
						return stbi__err("bad ctype", "Corrupt PNG");

					comp = stbi__get8(s);  
					if (comp) 
						return stbi__err("bad comp method", "Corrupt PNG");

					filter = stbi__get8(s);  
					if (filter) 
						return stbi__err("bad filter method", "Corrupt PNG");

					interlace = stbi__get8(s); 
					if (interlace > 1)
						return stbi__err("bad interlace method", "Corrupt PNG");

					if (!s->img_x || !s->img_y)
						return stbi__err("0-pixel image", "Corrupt PNG");

					if (!pal_img_n) {
						s->img_n = (color & 2 ? 3 : 1) + (color & 4 ? 1 : 0);
						if ((1 << 30) / s->img_x / s->img_n < s->img_y) return stbi__err("too large", "Image too large to decode");
					}
					else {
						// if paletted, then pal_n is our final components, and
						// img_n is # components to decompress/filter.
						s->img_n = 1;
						if ((1 << 30) / s->img_x / 4 < s->img_y) return stbi__err("too large", "Corrupt PNG");
					}
					// even with SCAN_header, have to scan to see if we have a tRNS
					break;
			}

			case STBI__PNG_TYPE('P', 'L', 'T', 'E'): {
				if (first) 
					return stbi__err("first not IHDR", "Corrupt PNG");

				if (c.length > 256 * 3) 
					return stbi__err("invalid PLTE", "Corrupt PNG");

				pal_len = c.length / 3;
				if (pal_len * 3 != c.length) 
					return stbi__err("invalid PLTE", "Corrupt PNG");

				for (i = 0; i < pal_len; ++i) {
					palette[i * 4 + 0] = stbi__get8(s);
					palette[i * 4 + 1] = stbi__get8(s);
					palette[i * 4 + 2] = stbi__get8(s);
					palette[i * 4 + 3] = 255;
				}

				break;
			}

			case STBI__PNG_TYPE('t', 'R', 'N', 'S'): {
				if (first) 
					return stbi__err("first not IHDR", "Corrupt PNG");

				if (z->idata)
					return stbi__err("tRNS after IDAT", "Corrupt PNG");

				if (pal_img_n) {
					if (scan == STBI__SCAN_header) {
						s->img_n = 4; 
						return 1;
					}

					if (pal_len == 0) 
						return stbi__err("tRNS before PLTE", "Corrupt PNG");

					if (c.length > pal_len)
						return stbi__err("bad tRNS len", "Corrupt PNG");

					pal_img_n = 4;

					for (i = 0; i < c.length; ++i)
						palette[i * 4 + 3] = stbi__get8(s);
				}
				else {
					if (!(s->img_n & 1)) 
						return stbi__err("tRNS with alpha", "Corrupt PNG");

					if (c.length != (stbi__uint32)s->img_n * 2) 
						return stbi__err("bad tRNS len", "Corrupt PNG");

					has_trans = 1;
					// non-paletted with tRNS = constant alpha. if header-scanning, we can stop now.
					if (scan == STBI__SCAN_header) {
						++s->img_n; 
						return 1; 
					}

					if (z->depth == 16)
						for (k = 0; k < s->img_n && k < 3; ++k) // extra loop test to suppress false GCC warning
							tc16[k] = (stbi__uint16)stbi__get16be(s); // copy the values as-is
					else
						for (k = 0; k < s->img_n && k < 3; ++k)
							tc[k] = (stbi_uc)(stbi__get16be(s) & 255) * stbi__depth_scale_table[z->depth]; // non 8-bit images will be larger
				}
				break;
			}

			case STBI__PNG_TYPE('I', 'D', 'A', 'T'): {
				if (first) 
					return stbi__err("first not IHDR", "Corrupt PNG");

				if (pal_img_n && !pal_len) 
					return stbi__err("no PLTE", "Corrupt PNG");

				if (scan == STBI__SCAN_header) {
					// header scan definitely stops at first IDAT
					if (pal_img_n)
						s->img_n = pal_img_n;
					return 1;
				}

				if (c.length > (1u << 30)) 
					return stbi__err("IDAT size limit", "IDAT section larger than 2^30 bytes");

				if ((int)(ioff + c.length) < (int)ioff)
					return 0;

				if (ioff + c.length > idata_limit) {
					stbi__uint32 idata_limit_old = idata_limit;
					stbi_uc* p;

					if (idata_limit == 0) idata_limit = c.length > 4096 ? c.length : 4096;
					while (ioff + c.length > idata_limit)
						idata_limit *= 2;

					STBI_NOTUSED(idata_limit_old);
					p = (stbi_uc*)STBI_REALLOC_SIZED(z->idata, idata_limit_old, idata_limit);

					if (p == NULL) 
						return stbi__err("outofmem", "Out of memory");

					z->idata = p;
				}

				if (!stbi__getn(s, z->idata + ioff, c.length)) 
					return stbi__err("outofdata", "Corrupt PNG");

				ioff += c.length;
				break;
			}

			case STBI__PNG_TYPE('I', 'E', 'N', 'D'): {
				stbi__uint32 raw_len, bpl;

				if (first) 
					return stbi__err("first not IHDR", "Corrupt PNG");

				if (scan != STBI__SCAN_load) 
					return 1;

				if (z->idata == NULL)
					return stbi__err("no IDAT", "Corrupt PNG");

				// initial guess for decoded data size to avoid unnecessary reallocs
				bpl = (s->img_x * z->depth + 7) / 8; // bytes per line, per component

				raw_len = bpl * s->img_y * s->img_n /* pixels */ + s->img_y /* filter mode per row */;

				z->expanded = (stbi_uc*)stbi_zlib_decode_malloc_guesssize_headerflag((char*)z->idata, ioff, raw_len, (int*)&raw_len, !is_iphone);
				if (z->expanded == NULL) 
					return 0; // zlib should set error

				free_null(z->idata);

				if ((req_comp == s->img_n + 1 && req_comp != 3 && !pal_img_n) || has_trans)
					s->img_out_n = s->img_n + 1;
				else
					s->img_out_n = s->img_n;

				if (!stbi__create_png_image(z, z->expanded,
					raw_len, s->img_out_n, z->depth, color, interlace)) 
					return 0;

				if (has_trans)
					if (z->depth == 16)
						if (!stbi__compute_transparency16(z, tc16, s->img_out_n)) 
							return 0;
					else
						if (!stbi__compute_transparency(z, tc, s->img_out_n)) 
							return 0;

				if (is_iphone && stbi__de_iphone_flag && s->img_out_n > 2)
					stbi__de_iphone(z);

				if (pal_img_n) {
					// pal_img_n == 3 or 4
					s->img_n = pal_img_n; // record the actual colors we had
					s->img_out_n = pal_img_n;

					if (req_comp >= 3) 
						s->img_out_n = req_comp;

					if (!stbi__expand_png_palette(z, palette, pal_len, s->img_out_n))
						return 0;
				}
				else if (has_trans)
					// non-paletted image with tRNS -> source image has (constant) alpha
					++s->img_n;
				
				free_null(z->expanded);
				// end of PNG chunk, read and skip CRC
				stbi__get32be(s);
				return 1;
			}

			default:
				// if critical, fail
				if (first) 
					return stbi__err("first not IHDR", "Corrupt PNG");

				if ((c.type & (1 << 29)) == 0) {
#ifndef STBI_NO_FAILURE_STRINGS
					// not threadsafe
					static char invalid_chunk[] = "XXXX PNG chunk not known";

					invalid_chunk[0] = STBI__BYTECAST(c.type >> 24);
					invalid_chunk[1] = STBI__BYTECAST(c.type >> 16);
					invalid_chunk[2] = STBI__BYTECAST(c.type >> 8);
					invalid_chunk[3] = STBI__BYTECAST(c.type >> 0);
#endif
					return stbi__err(invalid_chunk, "PNG not supported: unknown PNG chunk type");
				}
				stbi__skip(s, c.length);
				break;
			}
			// end of PNG chunk, read and skip CRC
			stbi__get32be(s);
		}

	}
	void* ImageReaderPrivate::doPng(
		stbi__png* p, 
		ImageData* data,
		stbi__result_info* ri)
	{
		void* result = NULL;

		if (parsePng(p, STBI__SCAN_load, STBI_AUTO_DETECT_CHANNELS_COUNT)) {
			if (p->depth <= 8)
				ri->bits_per_channel = 8;
			else if (p->depth == 16)
				ri->bits_per_channel = 16;
			else
				return stbi__errpuc("bad bits_per_channel", "PNG not supported: unsupported color depth");

			result = p->out;
			p->out = NULL;

			data->width = p->s->img_x;
			data->height = p->s->img_y;
			data->channels = p->s->img_n;

			data->depth = p->depth;
			data->bitsPerChannel = ri->bits_per_channel;
		}

		free_null(p->out);
		free_null(p->expanded);
		free_null(p->idata);

		return result;
	}

	void* ImageReaderPrivate::loadPng(
		stbi__context* s,
		ImageData* data,
		stbi__result_info* ri)
	{
		stbi__png p;
		p.s = s;

		void* pngData = doPng(&p, data, ri);

		data->colorSpace = ri->channel_order == STBI_ORDER_RGB
			? ColorSpace::RGB
			: ColorSpace::BGR; // ? TODO

		return pngData;
	}
#endif

#ifndef LIB_BASE_IMAGES_NO_JPEG
	stbi_uc* ImageReaderPrivate::loadJpegImage(
		stbi__jpeg* z, 
		ImageData* data)
	{
		int n, decode_n, is_rgb;
		z->s->img_n = 0; // make stbi__cleanup_jpeg safe

		// load a jpeg image from whichever source, but leave in YCbCr format
		if (!stbi__decode_jpeg_image(z)) { 
			stbi__cleanup_jpeg(z);
			return NULL; 
		}

		// determine actual number of components to generate
		n = z->s->img_n >= 3 ? 3 : 1;

		is_rgb = z->s->img_n == 3 && (z->rgb == 3 
			|| (z->app14_color_transform == 0 && !z->jfif));

		if (z->s->img_n == 3 && n < 3 && !is_rgb)
			decode_n = 1;
		else
			decode_n = z->s->img_n;

		// nothing to do if no components requested; check this now to avoid
		// accessing uninitialized coutput[0] later
		if (decode_n <= 0) { 
			stbi__cleanup_jpeg(z); 
			return NULL; 
		}

		// resample and color-convert
		{
			int k;
			unsigned int i, j;
			stbi_uc* output;
			stbi_uc* coutput[4] = { NULL, NULL, NULL, NULL };

			stbi__resample res_comp[4];

			for (k = 0; k < decode_n; ++k) {
				stbi__resample* r = &res_comp[k];

				// allocate line buffer big enough for upsampling off the edges
				// with upsample factor of 4
				z->img_comp[k].linebuf = (stbi_uc*)stbi__malloc(z->s->img_x + 3);
				if (!z->img_comp[k].linebuf) {
					stbi__cleanup_jpeg(z);
					return stbi__errpuc("outofmem", "Out of memory");
				}

				r->hs = z->img_h_max / z->img_comp[k].h;
				r->vs = z->img_v_max / z->img_comp[k].v;

				r->ystep = r->vs >> 1;
				r->w_lores = (z->s->img_x + r->hs - 1) / r->hs;

				r->ypos = 0;
				r->line0 = r->line1 = z->img_comp[k].data;

				if (r->hs == 1 && r->vs == 1)
					r->resample = resample_row_1;

				else if (r->hs == 1 && r->vs == 2)
					r->resample = stbi__resample_row_v_2;

				else if (r->hs == 2 && r->vs == 1)
					r->resample = stbi__resample_row_h_2;

				else if (r->hs == 2 && r->vs == 2)
					r->resample = z->resample_row_hv_2_kernel;

				else                            
					r->resample = stbi__resample_row_generic;
			}

			// can't error after this so, this is safe
			output = (stbi_uc*)stbi__malloc_mad3(n, z->s->img_x, z->s->img_y, 1);

			if (!output) {
				stbi__cleanup_jpeg(z);
				return stbi__errpuc("outofmem", "Out of memory"); 
			}

			// now go ahead and resample
			for (j = 0; j < z->s->img_y; ++j) {
				stbi_uc* out = output + n * z->s->img_x * j;

				for (k = 0; k < decode_n; ++k) {
					stbi__resample* r = &res_comp[k];
					int y_bot = r->ystep >= (r->vs >> 1);

					coutput[k] = r->resample(z->img_comp[k].linebuf,
						y_bot ? r->line1 : r->line0,
						y_bot ? r->line0 : r->line1,
						r->w_lores, r->hs);

					if (++r->ystep >= r->vs) {
						r->ystep = 0;
						r->line0 = r->line1;

						if (++r->ypos < z->img_comp[k].y)
							r->line1 += z->img_comp[k].w2;
					}
				}
				if (n >= 3) {
					stbi_uc* y = coutput[0];
					if (z->s->img_n == 3) {
						if (is_rgb) {
							for (i = 0; i < z->s->img_x; ++i) {
								out[0] = y[i];
								out[1] = coutput[1][i];
								out[2] = coutput[2][i];
								out[3] = 255;
								out += n;
							}
						}
						else {
							z->YCbCr_to_RGB_kernel(out, y, coutput[1], coutput[2], z->s->img_x, n);
						}
					}
					else if (z->s->img_n == 4) {
						if (z->app14_color_transform == 0) { // CMYK
							for (i = 0; i < z->s->img_x; ++i) {
								stbi_uc m = coutput[3][i];

								out[0] = stbi__blinn_8x8(coutput[0][i], m);
								out[1] = stbi__blinn_8x8(coutput[1][i], m);
								out[2] = stbi__blinn_8x8(coutput[2][i], m);

								out[3] = 255;
								out += n;
							}
						}
						else if (z->app14_color_transform == 2) { // YCCK
							z->YCbCr_to_RGB_kernel(out, y, coutput[1], coutput[2], z->s->img_x, n);
							for (i = 0; i < z->s->img_x; ++i) {
								stbi_uc m = coutput[3][i];

								out[0] = stbi__blinn_8x8(255 - out[0], m);
								out[1] = stbi__blinn_8x8(255 - out[1], m);
								out[2] = stbi__blinn_8x8(255 - out[2], m);

								out += n;
							}
						}
						else { // YCbCr + alpha?  Ignore the fourth channel for now
							z->YCbCr_to_RGB_kernel(out, y, coutput[1], coutput[2], z->s->img_x, n);
						}
					}
					else
						for (i = 0; i < z->s->img_x; ++i) {
							out[0] = out[1] = out[2] = y[i];
							out[3] = 255; // not used if n==3
							out += n;
						}
				}
				else {
					if (is_rgb) {
						if (n == 1)
							for (i = 0; i < z->s->img_x; ++i)
								*out++ = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
						else {
							for (i = 0; i < z->s->img_x; ++i, out += 2) {
								out[0] = stbi__compute_y(coutput[0][i], coutput[1][i], coutput[2][i]);
								out[1] = 255;
							}
						}
					}
					else if (z->s->img_n == 4 && z->app14_color_transform == 0) {
						for (i = 0; i < z->s->img_x; ++i) {
							stbi_uc m = coutput[3][i];

							stbi_uc r = stbi__blinn_8x8(coutput[0][i], m);
							stbi_uc g = stbi__blinn_8x8(coutput[1][i], m);
							stbi_uc b = stbi__blinn_8x8(coutput[2][i], m);

							out[0] = stbi__compute_y(r, g, b);
							out[1] = 255;

							out += n;
						}
					}
					else if (z->s->img_n == 4 && z->app14_color_transform == 2) {
						for (i = 0; i < z->s->img_x; ++i) {
							out[0] = stbi__blinn_8x8(255 - coutput[0][i], coutput[3][i]);
							out[1] = 255;
							out += n;
						}
					}
					else {
						stbi_uc* y = coutput[0];
						if (n == 1)
							for (i = 0; i < z->s->img_x; ++i) out[i] = y[i];
						else
							for (i = 0; i < z->s->img_x; ++i) { *out++ = y[i]; *out++ = 255; }
					}
				}
			}
			stbi__cleanup_jpeg(z);

			data->width = z->s->img_x;
			data->height = z->s->img_y;
			data->channels = z->s->img_n >= 3 ? 3 : 1; // report original components, not output

			return output;
		}
	}

	void* ImageReaderPrivate::loadJpeg(
		stbi__context* s,
		ImageData* data)
	{
		stbi__jpeg* jpeg = (stbi__jpeg*)stbi__malloc(sizeof(stbi__jpeg));
		if (!jpeg)
			return stbi__errpuc("outofmem", "Out of memory");

		memset(jpeg, 0, sizeof(stbi__jpeg));
		jpeg->s = s;

		stbi__setup_jpeg(jpeg);

		uchar* result = loadJpegImage(
			jpeg, &data->width, &data->height,
			(int32*)&data->channels, STBI_AUTO_DETECT_CHANNELS_COUNT);

		data->depth = JPEG_FORMAT_DEPTH;
		data->jpegQuality = kDefaultStbiJpegQuality;

		free(jpeg);
		return result;
	}
#endif

#ifndef LIB_BASE_IMAGES_NO_BMP
	void* ImageReaderPrivate::loadBmp(
		stbi__context* s,
		ImageData* data)
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

		data->width = s->img_x;
		data->height = s->img_y;

		data->bitsPerChannel = info.bpp;

		return out;
	}
#endif

	void* ImageReaderPrivate::loadImageMain(
		stbi__context* context,
		ImageData* data,
		stbi__result_info* ri,
		int32 bitsPerChannel)
	{
		measureExecutionTime("base::images::Image::loadImageMain")
		memset(ri, 0, sizeof(*ri));

		ri->bits_per_channel = 8;
		ri->channel_order = STBI_ORDER_RGB;
		ri->num_channels = 0;

#ifndef LIB_BASE_IMAGES_NO_PNG
		if (stbi__png_test(context)) {
			data->handler = new PngHandler();
			data->imageExtension = "png";

			return loadPng(context, data, ri);
		}
#endif
#ifndef LIB_BASE_IMAGES_NO_BMP
		if (stbi__bmp_test(context)) {
			data->handler = new BmpHandler();
			data->imageExtension = "bmp";

			return loadBmp(context, data);
		}
#endif
#ifndef LIB_BASE_IMAGES_NO_JPEG
		if (stbi__jpeg_test(context)) {
			data->handler = new JpegHandler();
			data->imageExtension = "jpeg";

			return loadJpeg(context, data);
		}
#endif

		return stbi__errpuc("unknown image type", "Image not of any known or supported type, or corrupt");
	}

	uchar* ImageReaderPrivate::loadAndPostprocess8Bit(
		stbi__context* context,
		ImageData* data)
	{
		stbi__result_info resultInfo;
		void* result = loadImageMain(context, data, &resultInfo, 8);

		if (result == NULL)
			return NULL;

		STBI_ASSERT(resultInfo.bits_per_channel == 8 || resultInfo.bits_per_channel == 16);

		if (resultInfo.bits_per_channel != 8) {
			measureExecutionTime("base::images::Image::loadAndPostprocess8Bit - resultInfo.bits_per_channel != 8: ")
				result = stbi__convert_16_to_8((stbi__uint16*)result,
					data->width, data->height, data->channels);

			resultInfo.bits_per_channel = 8;
		}

		if (stbi__vertically_flip_on_load) {
			measureExecutionTime("base::images::Image::loadAndPostprocess8Bit - stbi__vertically_flip_on_load: ")

			stbi__vertical_flip(result, data->width, data->height, data->channels * sizeof(stbi_uc));
		}

		return (uchar*)result;
	}

	uchar* ImageReaderPrivate::loadImage(
		FILE* file,
		ImageData* data)
	{
		stbi__context context;
		stbi__start_file(&context, file);

		uchar* result = loadAndPostprocess8Bit(&context, data);

		if (result) {
			fseek(file, -(int)(context.img_buffer_end - context.img_buffer), SEEK_CUR);
			data->dataLength = strlen((const char*)result) + 1;
		}

		return result;
	}

	void ImageReaderPrivate::ReadImage(ImageData* data)
	{
		if (data == nullptr || data->path.has_value() == false)
			return;

		FILE* file = stbi__fopen(data->path.value(), "rb");
		AssertLog(file != nullptr, "base::images::Image::readImage: Cannot fopen. Unable to open file");

		data->data = loadImage(file, data);
		data->sizeInBytes = ftell(file);

		data->bytesPerLine = Utility::CountBytesPerLine(data);

		fclose(file);
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
