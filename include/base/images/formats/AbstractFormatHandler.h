#pragma once 

#include <base/images/Images.h>
#include <base/utility/Algorithm.h>


namespace base::images {
	class AbstractFormatHandler {
	public:
		virtual void write(
			ImageData* data,
			const char* path) = 0;

		virtual void read(ImageData* data) = 0;

		virtual void convertToFormat(
			ImageData* data, 
			const char* format) = 0;

		virtual [[nodiscard]] const char* format() const noexcept { return "Unknown"; }
	};
} // namespace base::images
