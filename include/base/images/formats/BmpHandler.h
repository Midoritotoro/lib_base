#pragma once 

#include <base/images/formats/AbstractFormatHandler.h>
#include <base/io/File.h>


namespace base::images {
	class BmpHandler final: public AbstractFormatHandler {
	public:
		void write(
			ImageData* data,
			const char* path) override;

		void read(ImageData* data) override;

		void convertToFormat(
			ImageData* data,
			const char* format) override;

		[[nodiscard]] const char* format() const noexcept override;
	};
} // namespace base::images