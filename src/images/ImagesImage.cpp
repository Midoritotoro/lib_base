#include <base/images/ImagesImage.h>

#include <base/images/ImagesUtility.h>
#include <base/core/utility/Algorithm.h>

#include <base/images/ImageReader.h>
#include <base/images/filters/ImageFilter.h>

#include <base/images/formats/BmpHandler.h>
#include <base/images/formats/JpegHandler.h>

#include <base/images/formats/PngHandler.h>


__BASE_IMAGES_NAMESPACE_BEGIN

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

Image::Image(uchar* data) :
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

Image::Image(ImageData* data):
	_data(data)
{}

#ifdef LIB_BASE_ENABLE_QT
Image::Image(QImage&& image) :
	_data(new ImageData())
{
	_data->data = image.bits();
	_data->depth = image.depth();

	_data->sizeInBytes = image.sizeInBytes();

	_data->channels = Utility::GetChannelsCountByFormat(image);

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

	//_data->data = stbi_load_from_memory(
	//	data, length, &_data->width,
	//	&_data->height, (int32*)&_data->channels, FORCE_IMAGE_CHANNELS);

	//AssertLog(_data->data != nullptr, "base::images::Image::loadFromData: Cannot load image from memory");

	//_data->sizeInBytes = length;
	//_data->bytesPerLine = CountBytesPerLine(_data);
}

void Image::loadFromFile(const char* path) {
	if (path == nullptr)
		return;

	_data->path = path;
	ImageReader(_data->path.value()).read(this);

/*	AssertLog(_data->data != nullptr, 
		std::string("base::images::Image::loadFromFile: Cannot load image from file: "
		+ std::string(path)).c_str());*/
}

void Image::load(const char* path) {
	loadFromFile(path);
}

Image::~Image() {
	if (isNull() == false)
		free(_data->data);

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
}

void Image::resize(Size<int32> size) {
	return resize(size.width(), size.height());
}

std::vector<const char*> Image::supportedFormats() noexcept {
	return std::vector<const char*>({ "jpeg", "jpg", "png", "bmp" });
}

void Image::convertToFormat(const char* format) {
	_data->handler->convertToFormat(_data, format);
}

const char* Image::format() const noexcept {
	return _data->handler->format();
}

void Image::setFilter(Filter filter){
	ImageFilter(this).filter(filter);
}
	
Filter Image::filter() const noexcept {
	return _data->filter;
}

bool Image::hasFilter() const noexcept {
	return _data->filter != Filter::None;
}

void Image::save(const char* path) {
	AssertLog(!isNull(), std::string("base::images::Image::save: Cannot save a null image to path - " + std::string(path)).c_str());
	_data->handler->write(_data, path);
}

void Image::setJpegQuality(ushort quality) {
	_data->jpegQuality = quality;
}

ushort Image::jpegQuality() const noexcept {
	return _data->jpegQuality.value();
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

ImageData* Image::data_ptr() {
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
	if (!_data || x < 0 || x >= _data->width || y < 0 || y >= _data->height)
		return -1;
		
	return -1;
}

int32 Image::depth() const noexcept {
	return _data->depth;
};

ushort Image::channels() const noexcept {
	return _data->channels;
}

int32 Image::sizeInBytes() const noexcept {
	return _data->sizeInBytes;
}

sizetype Image::dataLength() const noexcept {
	return _data->dataLength;
}

int32 Image::bitsPerChannel() const noexcept {
	return _data->bitsPerChannel;
}

ColorSpace Image::colorSpace() const noexcept {
	return _data->colorSpace;
}

bool Image::hasPath() const noexcept {
	return _data->path.has_value();
}

const char* Image::path() const noexcept {
	return hasPath() ? _data->path.value() : "";
}

bool Image::isEqual(const Image& other) const {
	return _data->bytesPerLine == other._data->bytesPerLine
		&& _data->data == other._data->data
		&& _data->depth == other._data->depth
		&& _data->sizeInBytes == other._data->sizeInBytes
		&& _data->width == other._data->width
		&& _data->height == other._data->height
		&& _data->filter == other._data->filter
		&& strcmp(_data->handler->format(), other._data->handler->format()) == 0;
}

__BASE_IMAGES_NAMESPACE_END
