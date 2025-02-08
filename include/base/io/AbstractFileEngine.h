#pragma once 

#include <base/system/Platform.h>
#include <base/io/IOConfig.h>

#include <vector>


namespace base::io {
	class AbstractFileEngine {
	public:
		virtual void setFileName(const std::string& path) = 0;
		virtual void setFileDescriptor(not_null<FILE*> file) = 0;

		virtual void close() = 0;

		virtual [[nodiscard]] FILE* fileDescriptor() const noexcept = 0;
		virtual [[nodiscard]] std::string path() const noexcept = 0;

		virtual [[nodiscard]] bool isOpened() const noexcept = 0;

		virtual [[nodiscard]] std::string absolutePathFromDescriptor(FILE* descriptor) = 0;

		virtual [[nodiscard]] bool open(
			const std::string& path,
			FileOpenModes mode) = 0;
		virtual [[nodiscard]] bool open(
			const std::string& path,
			const std::string& mode) = 0;

		virtual [[nodiscard]] bool rename(const std::string& newFileName) = 0;

		virtual [[nodiscard]] bool rewind(int64 position) = 0;
		virtual [[nodiscard]] bool rewind(FilePositions position) = 0;

		virtual void remove() = 0;

		virtual [[nodiscard]] sizetype read(
			_SAL2_Out_writes_bytes_(sizeInBytes) void* outBuffer,
			_SAL2_In_ sizetype sizeInBytes) = 0;
	};
} // namespace base::io