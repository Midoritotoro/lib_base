#include <base/images/ImagesImage.h>
#include <base/OverflowCheck.h>

#include <base/Assert.h>
#ifdef LIB_BASE_ENABLE_QT
#include <base/images/ImagesUtility.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <base/images/StbImage.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <base/images/StbImageWrite.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <base/images/StbImageResize.h>


#include <base/Utility.h>
#include <qDebug>



namespace base::images {
	Image::Image() :
		_data(new ImageData())
	{}

	Image::Image(const std::string& path) :
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
		_data->depth = 1;

		_data->imageExtension = GetImageExtension(_data->data);
		_data->bytesPerLine = recountBytesPerLine(_data->width, _data->height, 1);
	}

	void Image::loadFromFile(const std::string& path) {
		_data->path = path;
		readImage(_data, kForceImageChannels);

		AssertLog(_data->data != nullptr, std::string("base::images::Image::loadFromFile: Cannot load image from file: " + path).c_str());

		_data->depth = 1;

		_data->bytesPerLine = recountBytesPerLine(_data->width, _data->height, 1);
		_data->imageExtension = GetImageExtension(_data->data);

		qDebug() << "_data->sizeInBytes: " << _data->sizeInBytes << "_data->w&h: " << _data->width
				 << _data->height << "_data->bytesPerLine: " << _data->bytesPerLine;
	}

	Image::~Image() {
		stbi_image_free(_data->data);
		delete base::take(_data);
	}

	Image Image::convertToColorSpace(ColorSpace space) const {
		return Image();
	}

	void Image::resize(int32 width, int32 height) {
		stbir_resize_uint8(
			_data->data, _data->width, _data->height, 0,
			_data->data, width, height, 0, _data->channels);

		_data->width = width;
		_data->height = height;

		_data->sizeInBytes = sizeof(*_data->data);
		_data->bytesPerLine = recountBytesPerLine(width, height, 1);
	}

	void Image::resize(Size<int32> size) {
		return resize(size.width(), size.height());
	}

	void Image::save(const std::string& path) {
		Assert(!isNull());

		qDebug() << "_data->channels: " << _data->channels;
		if (_data->imageExtension == "png")
			stbi_write_png(
				path.c_str(), _data->width, _data->height,
				_data->channels, _data->data, 0);
		else if (_data->imageExtension == "jpeg")
			stbi_write_jpg(
				path.c_str(), _data->width, _data->height,
				_data->channels, _data->data, 10000); // Quality - ?
		else if (_data->imageExtension == "bmp")
			stbi_write_bmp(
				path.c_str(), _data->width, _data->height,
				_data->channels, _data->data);
		else if (_data->imageExtension == "tga")
			stbi_write_tga(
				path.c_str(), _data->width, _data->height,
				_data->channels, _data->data);
		
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

		const uchar* s = _data->data + y * _data->bytesPerLine;

		int index = -1;
		switch (_data->colorSpace) {
		case ColorSpace::Mono:
			index = (*(s + (x >> 3)) >> (~x & 7)) & 1;
			break;
		}

		if (index >= 0) {    // Indexed format
			if (index >= _data->colorTable.size()) {
				qWarning("QImage::pixel: color table index %d out of range.", index);
				return 0;
			}
			return _data->colorTable.at(index);
		}

		switch (_data->colorSpace) {
		case ColorSpace::RGB:
			return 0xff000000 | reinterpret_cast<const Rgb*>(s)[x];
		case ColorSpace::RGBA:
			return reinterpret_cast<const Rgb*>(s)[x];
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

	int32 Image::recountBytesPerLine(
			int32 width, int32 height, int32 depth)
	{
		int32 invalid = -1;

		if (height <= 0)
			return invalid;

		auto _bytesPerLine = sizetype(0);

		if (MultiplyOverflow(width, depth, &_bytesPerLine))
			return invalid;
		if (AdditionOverflow(_bytesPerLine, sizetype(31), &_bytesPerLine))
			return invalid;

		_bytesPerLine = (_bytesPerLine >> 5) << 2;    // can't overflow

		auto dummy = sizetype(0);
		if (MultiplyOverflow(height, sizetype(sizeof(uchar*)), &dummy))
			return invalid;

		if (width > (INT_MAX - 31) / depth)
			return invalid;

		return _bytesPerLine;
	}

	void Image::readImage(
		ImageData* data,
		int32 forceChannelsCount)
	{
		if ((_data == nullptr && _data->data == nullptr)
			|| data->path.has_value() == false)
			return;

		FILE* file = stbi__fopen(data->path.value().c_str(), "rb");
		AssertLog(file != nullptr, "base::images::Image::readImage: Cannot fopen. Unable to open file");

		_data->data = stbi_load_from_file(
			file, &data->width, &data->height,
			(int32*)&data->channels, forceChannelsCount);

		data->sizeInBytes = ftell(file);
		fclose(file);
	}

	void* Image::loadImageMain(
		stbi__context* context,
		ImageData* data,
		int32 forceChannelsCount,
		stbi__result_info* ri,
		int32 bitsPerChannel)
	{
		memset(ri, 0, sizeof(*ri));

		ri->bits_per_channel = 8;
		ri->channel_order = STBI_ORDER_RGB;
		ri->num_channels = 0;

		if (stbi__png_test(context)) {
			data->imageExtension = "png";
			return stbi__png_load(
				context, &data->width, &data->height,
				(int32*)&data->channels, forceChannelsCount, ri);
		}

		if (stbi__bmp_test(context)) {
			data->imageExtension = "bmp";
			return stbi__bmp_load(
				context, &data->width, &data->height,
				(int32*)&data->channels, forceChannelsCount, ri);
		}

		if (stbi__gif_test(context)) {
			data->imageExtension = "gif";
			return stbi__gif_load(
				context, &data->width, &data->height,
				(int32*)&data->channels, forceChannelsCount, ri);
		}

		if (stbi__psd_test(context)) {
			data->imageExtension = "psd";
			return stbi__psd_load(
				context, &data->width, &data->height,
				(int32*)&data->channels, forceChannelsCount, ri, bitsPerChannel);
		}

		if (stbi__pic_test(context)) {
			data->imageExtension = "pic";
			return stbi__pic_load(
				context, &data->width, &data->height,
				(int32*)&data->channels, forceChannelsCount, ri);
		}
	
		if (stbi__jpeg_test(context)) {
			data->imageExtension = "jpeg";
			return stbi__jpeg_load(
				context, &data->width, &data->height,
				(int32*)&data->channels, forceChannelsCount, ri);
		}

		if (stbi__pnm_test(context)) {
			data->imageExtension = "pnm";
			return stbi__pnm_load(
				context, &data->width, &data->height,
				(int32*)&data->channels, forceChannelsCount, ri);
		}

		if (stbi__hdr_test(context)) {
			data->hdr = true;
			float* hdr = stbi__hdr_load(
				context, &data->width, &data->height,
				(int32*)&data->channels, forceChannelsCount, ri);

			return stbi__hdr_to_ldr(
				hdr, data->width, data->height,
				forceChannelsCount ? forceChannelsCount : data->channels);
		}

		if (stbi__tga_test(context)) {
			data->imageExtension = "tga";
			return stbi__tga_load(
				context, &data->width, &data->height,
				(int32*)&data->channels, forceChannelsCount, ri);
		}

		return stbi__errpuc("unknown image type", "Image not of any known type, or corrupt");
	}

	uchar* Image::loadAndPostprocess8Bit(
		stbi__context* context,
		ImageData* data,
		int32 forceChannelsCount)
	{
		stbi__result_info resultInfo;
		void* result = loadImageMain(context, data, forceChannelsCount, &resultInfo, 8);

		if (result == NULL)
			return NULL;

		STBI_ASSERT(resultInfo.bits_per_channel == 8 || resultInfo.bits_per_channel == 16);

		if (resultInfo.bits_per_channel != 8) {
			result = stbi__convert_16_to_8((stbi__uint16*)result, 
				data->width, data->height, 
				forceChannelsCount == 0 
					? data->channels 
					: forceChannelsCount);

			resultInfo.bits_per_channel = 8;
		}

		if (stbi__vertically_flip_on_load) {
			int channels = forceChannelsCount ? forceChannelsCount : data->channels;
			stbi__vertical_flip(result, data->width, data->height, channels * sizeof(stbi_uc));
		}

		return (uchar*)result;
	}

	uchar* Image::loadImage(
		FILE* file,
		ImageData* data,
		int32 forceChannelsCount)
	{
		stbi__context context;
		stbi__start_file(&context, file);

		uchar* result = loadAndPostprocess8Bit(
			&context, data, forceChannelsCount);

		if (result)
			fseek(file, -(int)(context.img_buffer_end - context.img_buffer), SEEK_CUR);

		return result;
	}

	void Image::writeImageToFile(
		ImageData* data,
		const std::string& path)
	{
		
	}
} // namespace base::images