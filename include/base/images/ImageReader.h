#pragma once 

#include <base/images/Images.h>


namespace base::images {
	class ImageReader {
	public:
		ImageReader();
		~ImageReader();

		void read(
			ImageData* data,
			const char* path);

		void write(
			ImageData* data,
			const char* path);

		void convertToFormat(
			ImageData* data,
			const char* format);
	};
}