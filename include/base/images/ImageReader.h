#pragma once 

#include <base/images/Images.h>

__BASE_IMAGES_NAMESPACE_BEGIN

class Image;
class ImageReaderPrivate;

class ImageReader {
public:
	ImageReader(const char* path);

	void read(Image* image);
private:
	ImageReaderPrivate* _private = nullptr;
};

__BASE_IMAGES_NAMESPACE_END