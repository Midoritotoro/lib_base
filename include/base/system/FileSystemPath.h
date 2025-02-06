#pragma once

#include <string>


namespace base::system {
	class FileSystemPath {
	public:
		static [[nodiscard]] bool isAbsolutePath(const std::string& path);
		static [[nodiscard]] bool isRelativePath(const std::string& path);
	};
} // namespace base::system