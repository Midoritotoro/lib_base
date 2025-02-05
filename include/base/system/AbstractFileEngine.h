#pragma once 

#include <base/system/Platform.h>
#include <vector>


namespace base::system {
	class AbstractFileEngine {
	public:
		virtual ~AbstractFileEngine() = 0;

		virtual void setFileName(const std::string& path) = 0;
		virtual void setFileDescriptor(not_null<FILE*> file) = 0;

		virtual [[nodiscard]] bool exists(const std::string& path) = 0;
		virtual [[nodiscard]] std::vector<std::string>
			find(
				const std::string& path,
				const FileFilter& filter,
				bool recurse = true) = 0;

		virtual [[nodiscard]] FILE* fileDescriptor() const noexcept = 0;
		virtual [[nodiscard]] std::string path() const noexcept = 0;

		virtual [[nodiscard]] std::string absolutePathFromDescriptor(FILE* descriptor) = 0;
	};
} // namespace base::system