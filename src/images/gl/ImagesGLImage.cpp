#include <base/images/gl/ImagesGLImage.h>
#include <base/OverflowCheck.h>

#include <base/Assert.h>


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

		_data->channels = image.format() == QImage::Format::

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