#include <base/images/gl/ImagesGLImage.h>
#include <base/OverflowCheck.h>

#include <base/Assert.h>
#include <private/qpixellayout_p.h>

#ifdef LIB_BASE_ENABLE_QT
#include <base/images/ImagesQtUtility.h>
#endif

#include <base/Utility.h>


namespace base::images {
	GLImage::GLImage():
		_data(new GLImageData())
	{}

	GLImage::GLImage(GLImage&& image):
		_data(image._data)
	{}

	GLImage::GLImage(const IntegralImage& image) {

	}

	GLImage::GLImage(::uchar* data):
		_data(new GLImageData())
	{
		_data->data = data;
	}

	GLImage::GLImage(const Size<int32>& size):
		_data(new GLImageData())
	{
		_data->width = size.width();
		_data->height = size.height();
	}

	GLImage::GLImage(int32 width, int32 height):
		_data(new GLImageData())
	{
		_data->width = width;
		_data->height = height;
	}

	GLImage::GLImage(
		::uchar* data,
		int32 width, int32 height
	):
		_data(new GLImageData()) 
	{
		_data->data = data;

		_data->width = width;
		_data->height = height;
	}

	GLImage::GLImage(
		::uchar* data,
		const Size<int32>& size
	):
		_data(new GLImageData())

	{
		_data->data = data;

		_data->width = size.width();
		_data->height = size.height();
	}

#ifdef LIB_BASE_ENABLE_QT
	GLImage::GLImage(QImage&& image):
		_data(new GLImageData())
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

	bool GLImage::operator==(const GLImage& other) {
		return isEqual(other);
	}

	bool GLImage::operator!=(const GLImage& other) {
		return !isEqual(other);
	}

	void GLImage::loadFromData(uchar* data) {
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
		_data->depth = 32; // ???

		_data->bytesPerLine = recountImageParameters(width, height, 1).bytesPerLine;
	}

	void GLImage::loadFromFile(const std::string& path) {
		uchar* imageData = nullptr;
		int32 width = 0, height = 0, channels = 0;

		imageData = stbi_load(path.c_str(), &width, &height, &channels, kForceImageChannels);
		Assert(imageData != nullptr);

		_data->data = imageData;
		_data->totalSize = sizeof(*imageData);

		_data->width = width;
		_data->height = height;

		_data->channels = channels;
		_data->depth = 32; // ???

		_data->bytesPerLine = recountImageParameters(width, height, 1).bytesPerLine;
	}

	GLImage::~GLImage() {
		stbi_image_free(_data->data);
		delete base::take(_data);
	}

	GLImage GLImage::convertToColorSpace(ColorSpace space) const {

	}

	void GLImage::resize(int32 width, int32 height) {
		/*stbir_resize(_data->data, _data->width, _data->height, _data->totalSize,
				_data->data, width, height, 0, )*/
	}

	void GLImage::resize(Size<int32> size) {
		return resize(size.width(), size.height());
	}

	Rect<int32> GLImage::rect() const noexcept {
		return Rect<int32>(0, 0, _data->width, _data->height);
	}

	Size<int32> GLImage::size() const noexcept {
		return Size<int32>(_data->width, _data->height);
	}

	int32 GLImage::width() const noexcept {
		return _data->width;
	}

	int32 GLImage::height() const noexcept {
		return _data->height;
	}

	int32 GLImage::bytesPerLine() const noexcept {

	}

	uchar* GLImage::bytesData() {
		return _data->data;
	}

	GLImage::GLImageData* GLImage::data() {
		return _data;
	}

	uchar* GLImage::scanLine(int i)
	{
		if (!_data)
			return nullptr;

		return _data->data + i * _data->bytesPerLine;
	}

	const uchar* GLImage::scanLine(int i) const
	{
		if (!_data)
			return nullptr;

		Assert(i >= 0 && i < height());
		return _data->data + i * _data->bytesPerLine;
	}

	Rgb GLImage::pixel(int x, int y) const
	{
		if (!_data || x < 0 || x >= _data->width || y < 0 || y >= _data->height) {
			qWarning("QImage::pixel: coordinate (%d,%d) out of range", x, y);
			return 12345;
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
		case ColorSpace::ARGB32:
		case ColorSpace::ARGB32_Premultiplied:
			return reinterpret_cast<const Rgb*>(s)[x];
		}

		return -1;
	}

#ifdef defined(LIB_BASE_ENABLE_OPENGL) || defined(LIB_BASE_ENABLE_QT_OPENGL)
	void GLImage::paint() {

	}
#endif

	bool GLImage::isEqual(const GLImage& other) const {
		return _data->bytesPerLine == other._data->bytesPerLine
			&& _data->data == other._data->data
			&& _data->depth == other._data->depth
			&& _data->devicePixelRatio == other._data->devicePixelRatio
			&& _data->totalSize == other._data->totalSize
			&& _data->width == other._data->width
			&& _data->height == other._data->height;
	}

	GLImage::GLImageSizeParameters 
		GLImage::recountImageParameters(
			int32 width, int32 height, int32 depth)
		{
			GLImageSizeParameters invalid = { -1 };

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

			return GLImageSizeParameters {
				.bytesPerLine = _bytesPerLine
			};
		}
} // namespace base::images