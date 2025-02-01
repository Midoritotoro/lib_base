#pragma once 

#include <base/images/Images.h>


namespace base::images {
	class AbstractFormatHandler {
	public:
		virtual void write(
			ImageData* data,
			const char* path) = 0;

		virtual [[nodiscard]] AbstractFormatHandler* convertToFormat(
			ImageData* data, 
			const char* format) = 0;

		AbstractFormatHandler& operator=(const AbstractFormatHandler& other);

	};
} // namespace base::images
