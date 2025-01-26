#include <base/images/ImagesImage.h>
#include <base/OverflowCheck.h>

#include <base/Assert.h>
#ifdef LIB_BASE_ENABLE_QT
#include <base/images/ImagesQtUtility.h>
#endif

#define STB_IMAGE_IMPLEMENTATION
#include <base/images/StbImage.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <base/images/StbImageWrite.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <base/images/StbImageResize.h>


#include <base/Utility.h>


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
		::uchar* data,
		int32 width, int32 height
	) :
		_data(new ImageData())
	{
		_data->data = data;

		_data->width = width;
		_data->height = height;
	}

	Image::Image(
		::uchar* data,
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

		_data->totalSize = image.sizeInBytes();
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

	void Image::loadFromData(uchar* data) {
		int32 width = 0, height = 0, channels = 0;

		uchar* readedData = stbi_load_from_memory(
			data, sizeof(*data), &width,
			&height, &channels, kForceImageChannels);

		Assert(readedData != nullptr);

		_data->data = readedData;
		_data->totalSize = sizeof(*readedData);

		_data->width = width;
		_data->height = height;

		_data->channels = channels;
		_data->depth = 1; // ??? 32

		_data->bytesPerLine = recountBytesPerLine(width, height, 1);
	}

	void Image::loadFromFile(const std::string& path) {
		uchar* imageData = nullptr;
		int32 width = 0, height = 0, channels = 0;

		imageData = stbi_load(
			path.c_str(), &width, &height,
			&channels, kForceImageChannels);

		Assert(imageData != nullptr);

		_data->data = imageData;
		_data->totalSize = sizeof(*imageData);

		_data->width = width;
		_data->height = height;

		_data->channels = channels;
		_data->depth = 1; // ??? 32

		_data->bytesPerLine = recountBytesPerLine(width, height, 1);
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

		_data->totalSize = sizeof(*_data->data);
		_data->bytesPerLine = recountBytesPerLine(width, height, 1);
	}

	void Image::resize(Size<int32> size) {
		return resize(size.width(), size.height());
	}

	void Image::save(const std::string& path) {
		Assert(!isNull());

		stbi_write_png(path.c_str(), _data->width, _data->height, 0, _data->data, 0);
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

	Image::ImageData* Image::data() {
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
			|| (_data->data == nullptr || _data->totalSize <= 0)
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
		case ColorSpace::RGB32:
			return 0xff000000 | reinterpret_cast<const Rgb*>(s)[x];
		case ColorSpace::RGBA32:
			return reinterpret_cast<const Rgb*>(s)[x];
		}

		return -1;
	}

	bool Image::isEqual(const Image& other) const {
		return _data->bytesPerLine == other._data->bytesPerLine
			&& _data->data == other._data->data
			&& _data->depth == other._data->depth
			&& _data->devicePixelRatio == other._data->devicePixelRatio
			&& _data->totalSize == other._data->totalSize
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
} // namespace base::images