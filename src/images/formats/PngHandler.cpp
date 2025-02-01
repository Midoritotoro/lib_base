#include <base/images/formats/PngHandler.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <base/images/stb/StbImageWrite.h>

namespace base::images {
	void PngHandler::write(
		ImageData* data,
		const char* path)
	{

	}

	AbstractFormatHandler* PngHandler::convertToFormat(
		ImageData* data,
		const char* format)
	{

	}
} // namespace base::images