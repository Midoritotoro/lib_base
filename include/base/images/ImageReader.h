#pragma once 

#include <base/images/Images.h>

namespace base::images {
	class Image;
	class ImageReaderPrivate;

	class ImageReader {
	public:
		ImageReader(const char* path);

		void read(Image* image);
	private:
		ImageReaderPrivate* _private = nullptr;
	};
}