#pragma once


#if defined(OS_MAC) || defined(OS_LINUX)

#include <string>

namespace base::io {
	class UnixFileSystemPath {
	public:
		static [[nodiscard]] bool isAbsolutePath(const std::string& path);
		static [[nodiscard]] bool isRelativePath(const std::string& path);

		//!
		//! \return std::tuble(head, tail), где tail - это все, что находится после последней косой черты.
		//! Любая часть может быть пустой.
		static [[nodiscard]] std::pair<std::string, std::string>
			split(const std::string& path);

		static [[nodiscard]] std::string directoryName(const std::string& path);
		static [[nodiscard]] std::string baseName(const std::string& path);
	};
} // namespace base::io

#endif
